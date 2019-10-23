import java.util.ArrayList;
import java.util.Hashtable;
import java.util.concurrent.Callable;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Andreea
 */
public class ConditionOp implements Callable< ArrayList <Integer> > {
    Hashtable<String, Table> tables;
    String tableName;
    Condition c;
    int startInd, finInd;
    
    public ConditionOp(Hashtable<String, Table> tables, String tableName, Condition condition, int startInd, int finInd) {
        this.tables = tables;
        this.tableName = tableName;
        this.c = condition;
        this.startInd = startInd;
        this.finInd = finInd;
    }
    
   
    @Override
    public ArrayList<Integer> call() throws Exception {
        Table t = tables.get(tableName);
        ArrayList<Integer> indices = new ArrayList<>();
        boolean check = false;
        if(c.valueType.equals("any"))
        {
            for(int i = startInd; i< finInd;i++ )
            //for(int i= 1; i< t.columns.get(0).size();i++ )            //parcurg elem unei coloane (incep de la 1 pt ca la 0 am numele coloanei)
            {
                indices.add(i);
            }
            return indices;
        }

        Integer index = t.columnNames.get(c.columnName);
        c.validCond(t.columnTypes[index]);
        //for(int i = 1; i < t.columns.get(index).size(); i++)            //parcurg elem unei coloane (incep de la 1 pt ca la 0 am numele coloanei)
        ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! finIndex depinde de updateOp.t.columnNames.get(c.columnName); !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //TO DOOO
        for(int i = startInd; i <finInd; i++)
        {
            check = c.respectsCond( (t.columns.get(index)).get(i));
            if(check == true)
               indices.add(i);
        }
            return indices;
    }
    
}
