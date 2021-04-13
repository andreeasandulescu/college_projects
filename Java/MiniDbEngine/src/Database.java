
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.concurrent.ExecutionException;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Andreea
 */
public class Database implements MyDatabase {

    int nrTables;
    Hashtable<String, Table> tables;
    ExecutorService executor;
    int numWorkerThreads;

    public Database() {
        this.nrTables = 0;
        tables = new Hashtable<>();
        numWorkerThreads = 0;
    }

    @Override
    public void initDb(int numWorkerThreads) {
        ExecutorService executor = new ThreadPoolExecutor(numWorkerThreads, numWorkerThreads, 0, TimeUnit.HOURS, new ArrayBlockingQueue<>(9999));
        this.executor = executor;
        this.numWorkerThreads = numWorkerThreads;
    }

    @Override
    public void stopDb() {
        executor.shutdown();
    }

    @Override
    public void createTable(String tableName, String[] columnNames, String[] columnTypes) {
        Table t = new Table(columnNames, columnTypes);
        tables.put(tableName, t);
        nrTables++;
    }

    @Override
    public ArrayList<ArrayList<Object>> select(String tableName, String[] operations, String condition) {
        ArrayList<Future<ArrayList<Integer>>> ind = new ArrayList<>();
        ArrayList<Future<ArrayList<ArrayList<Object>>>> res = new ArrayList<>();
        ArrayList<ArrayList<Integer>> indices = new ArrayList<>();
        Operation[] ops = new Operation[operations.length];
        ArrayList<ArrayList<Object>> finalRes;
        Table t = tables.get(tableName);
        Condition c = new Condition(condition);
        int size = 0;

        for (int i = 0; i < operations.length; i++) {
            ops[i] = new Operation(operations[i]);
            ops[i].validOp(t.columnTypes[t.columnNames.get(ops[i].colName)]);
        }

        try {
            t.transactionSem.acquire();
        } catch (InterruptedException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
        }
        if (t.transaction == false) {
            try {
                t.transactionSem.release();
                t.readCntSem.acquire();
                t.readCnt++;
                if (t.readCnt == 1) {
                    t.resource.acquire();
                }
                t.readCntSem.release();
            } catch (InterruptedException ex) {
                ex.printStackTrace();
                throw new RuntimeException("Eroare in select, la semafor readCntSem");
            }
        } else {
            t.transactionSem.release();
            t.transactionLock.lock();
            t.transactionLock.unlock();
        }
        
        c.validCond(t.columnTypes[t.columnNames.get(c.columnName)]);
        int nrIt = t.columns.get(0).size() / numWorkerThreads;

        for (int i = 0; i < numWorkerThreads - 1; i++) {
            ind.add(executor.submit(new ConditionOp(tables, tableName, c, i * nrIt, (i + 1) * nrIt)));
        }
        ind.add(executor.submit(new ConditionOp(tables, tableName, c, (numWorkerThreads - 1) * nrIt, t.columns.get(0).size())));

        for (int i = 0; i < ind.size(); i++) {
            try {
                indices.add((ArrayList<Integer>) ind.get(i).get());
                size += indices.get(i).size();
            } catch (InterruptedException | ExecutionException ex) {
                ex.printStackTrace();
                throw new RuntimeException("Eroare in select!!!!!!");
            }
        }

        if (size > numWorkerThreads * 3) {
            for (int i = 0; i < numWorkerThreads - 1; i++) {
                res.add(executor.submit(new SelectOp(tables, tableName, c, ops, indices.get(i))));
            }
            res.add(executor.submit(new SelectOp(tables, tableName, c, ops, indices.get(numWorkerThreads - 1))));
        } else {
            res.add(executor.submit(new SelectOp(tables, tableName, c, ops, indices.get(0))));
        }

        try {
            finalRes = (new SelectResult(res, ops, size).call());
        } catch (Exception ex) {
            ex.printStackTrace();
            throw new RuntimeException("Eroare in select");
        }

        try {
            t.transactionSem.acquire();
        } catch (InterruptedException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
        }
        if (t.transaction == false) {
            try {
                t.transactionSem.release();
                t.readCntSem.acquire();
                t.readCnt--;
                if (t.readCnt == 0) {
                    t.resource.release();
                }
                t.readCntSem.release();
            } catch (InterruptedException ex) {
                ex.printStackTrace();
                throw new RuntimeException("Eroare in select, la semafor readCntSem");
            }
        }
        t.transactionSem.release();

        return finalRes;
    }

    @Override
    public void update(String tableName, ArrayList<Object> values, String condition) {
        Table t = tables.get(tableName);
        Condition c = new Condition(condition);
        ArrayList<Future> res = new ArrayList<>();
        int colLength;
        int nrIt;

        try {
            t.transactionSem.acquire();
        } catch (InterruptedException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
        }
        if (t.transaction == false) {
            try {
                t.transactionSem.release();
                t.resource.acquire();
            } catch (InterruptedException ex) {
                ex.printStackTrace();
                throw new RuntimeException("Eroare in update, la semafor resource");
            }
        } else {
            t.transactionSem.release();
            t.transactionLock.lock();
            t.transactionLock.unlock();
        }
        
        //scriere
        int index = t.columnNames.get(c.columnName);
        
        c.validCond(t.columnTypes[index]);
        colLength = t.columns.get(0).size();
        nrIt = colLength / numWorkerThreads;
        if (colLength > numWorkerThreads * 3) {
            for (int i = 0; i < numWorkerThreads - 1; i++) {
                res.add(executor.submit(new UpdateOp(tables, tableName, values, c, i * nrIt, (i + 1) * nrIt)));
            }
            res.add(executor.submit(new UpdateOp(tables, tableName, values, c, (numWorkerThreads - 1) * nrIt, colLength)));
        } else {
            res.add(executor.submit(new UpdateOp(tables, tableName, values, c, 0, colLength)));
        }

        for (int i = 0; i < res.size(); i++) {
            try {
                res.get(i).get();
            } catch (InterruptedException | ExecutionException ex) {
                Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
            }
        }

        try {
            t.transactionSem.acquire();
        } catch (InterruptedException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
        }
        if (t.transaction == false) {
            t.resource.release();
        }
        t.transactionSem.release();
    }

    @Override
    public void insert(String tableName, ArrayList<Object> values) {
        Table t = tables.get(tableName);
        
        try {
            t.transactionSem.acquire();
        } catch (InterruptedException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
        }
        if (t.transaction == false) {
            try {
                t.transactionSem.release();
                t.resource.acquire();
            } catch (InterruptedException ex) {
                ex.printStackTrace();
                throw new RuntimeException("Eroare in insert, la semafor resource");
            }
        } else {
            t.transactionSem.release();
            t.transactionLock.lock();
            t.transactionLock.unlock();
        }
        
        
        boolean checkInt, checkBool, checkString;
        for (int i = 0; i < t.columns.size(); i++) {
            checkInt = (values.get(i) instanceof Integer) && t.columnTypes[i].equals("int");
            checkBool = (values.get(i) instanceof Boolean) && t.columnTypes[i].equals("bool");
            checkString = (values.get(i) instanceof String) && t.columnTypes[i].equals("string");
            if (checkInt || checkBool || checkString) {
                t.columns.get(i).add(values.get(i));
            } else {
                throw new RuntimeException("Inserare element incompatibil cu tipul elem din coloana!");
            }
        }
        
        try {
            t.transactionSem.acquire();
        } catch (InterruptedException ex) {
            Logger.getLogger(Database.class.getName()).log(Level.SEVERE, null, ex);
        }
        if (t.transaction == false) {
            t.resource.release();
        }
        t.transactionSem.release();
        
    }

    @Override
    public void startTransaction(String tableName) {

        Table t = tables.get(tableName);
        try {
            t.transactionLock.lock();
            t.transactionSem.acquire();
            
            t.resource.acquire();
            t.transaction = true;
            t.transactionSem.release();
        } catch (InterruptedException ex) {
            ex.printStackTrace();
            throw new RuntimeException("Eroare in startTransaction");
        }

    }

    @Override
    public void endTransaction(String tableName) {
        Table t = tables.get(tableName);
        try {
            t.transactionLock.unlock();
            t.transactionSem.acquire();
            t.transaction = false;
            t.transactionSem.release();
            t.resource.release();

        } catch (InterruptedException ex) {
            ex.printStackTrace();
            throw new RuntimeException("Eroare in endTransaction");
        }

         
    }

}
