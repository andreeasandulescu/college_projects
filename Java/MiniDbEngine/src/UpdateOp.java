
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
public class UpdateOp implements Callable<Object>{
    Hashtable<String, Table> tables;
    String tableName;
    ArrayList<Object> values;
    Condition condition;
    int startInd, finInd;
    public UpdateOp(Hashtable<String, Table> tables, String tableName, ArrayList<Object> values, Condition condition, int startInd, int finInd)
    {
        this.tables = tables;
        this.tableName = tableName;
        this.values = values;
        this.startInd = startInd;
        this.finInd = finInd;
        this.condition = condition;
    }

    
    @Override
    public Object call() throws Exception {
       Table t = tables.get(tableName);
       int index = t.columnNames.get(condition.columnName);
        for(int i = startInd; i< finInd;i++ )
        {
            if( condition.respectsCond(t.columns.get(index).get(i)))
            {
                for(int  j = 0;j < t.columns.size(); j++)
                    (t.columns.get(j)).set(i, values.get(j));
            }
           
        }  
        return null;
    }
    
}
