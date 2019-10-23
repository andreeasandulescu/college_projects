import java.util.ArrayList;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Andreea
 */
public class Table {
    String[] columnTypes;
    ArrayList <ArrayList<Object>> columns;
    Hashtable<String, Integer> columnNames;
    Semaphore resource, readCntSem, transactionSem;
    Lock transactionLock;
    int readCnt;
    boolean transaction;
    
    public Table(String[] columnNames, String[] columnTypes) {
        readCnt = 0;
        transaction = false;
        resource = new Semaphore(1);
        readCntSem = new Semaphore(1);
        transactionSem = new Semaphore(1);
        this.columnTypes = Arrays.copyOf(columnTypes, columnTypes.length);
        this.columnNames = new Hashtable<>();
        transactionLock = new ReentrantLock();
        columns = new ArrayList <> ();
        for(int i = 0;i < columnTypes.length; i++)
        {
            this.columnNames.put(columnNames[i], i);        
            columns.add(new ArrayList<>());
        }
    }
    
    
    void printTable()
    {
        if(columnTypes.length <= 0)
            return;
        for(int i = 0 ;i < columns.get(0).size() ; i++)
        {
            for(int j = 0; j < columns.size() ;j++)
            {
                System.out.print(String.valueOf((columns.get(j)).get(i)) + ", ");
            }
            System.out.println(" ");
        }
    }
}

   
    


