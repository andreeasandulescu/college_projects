import java.util.ArrayList;
import java.util.Hashtable;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Andreea
 */
public class SelectOp implements Callable <ArrayList<ArrayList<Object>>> {
    Hashtable<String, Table> tables;
    String tableName;
    ArrayList <Integer> indices;
    Condition condition;
    Operation [] operations;

    public SelectOp(Hashtable<String, Table> tables, String tableName, Condition condition, Operation[] ops, ArrayList <Integer> ind) {
     
            int index;
            Table t = tables.get(tableName);
            this.tables = tables;
            this.tableName = tableName;
            this.condition = condition;
            operations = ops;
            indices = ind;
      
    }
    
    @Override
    public ArrayList<ArrayList<Object>> call() throws Exception {
       Table t = tables.get(tableName);
       ArrayList<ArrayList<Object>> arr = new ArrayList();
       ArrayList<Object> aux;
       
        for(int i = 0; i < operations.length;i++ )
        {
           aux = new ArrayList<>();
           switch(operations[i].op)
           {
               case "min":
                   aux.add(this.min(operations[i]));
                   arr.add(aux);
                   break;
               case "max":
                   aux.add(this.max(operations[i]));
                   arr.add(aux);
                   break;
                case "sum":
                   aux.add(this.sum(operations[i]));
                   arr.add(aux);
                   break;
                 case "count":
                   aux.add(this.count(operations[i]));
                   arr.add(aux);
                   break;
                case "avg":
                   aux.add(this.sum(operations[i]));
                   arr.add(aux);
                   break;
                case "none":
                    int index = t.columnNames.get(operations[i].colName);
                    for(int j = 0;j<indices.size();j++)
                        aux.add((t.columns.get(index)).get(indices.get(j)));
                    arr.add(aux);
                    break;
           }
        }  
        return arr;
    }
    
    public int sum(Operation op)
    {   
        int sum = 0, nr = 0;
        Table t = tables.get(tableName);
        int index = t.columnNames.get(op.colName);              //coloana la care se refera operatia
        for(int i=0;i<indices.size();i++)
        {
            nr = (int) (t.columns.get(index)).get(indices.get(i));
            sum+=nr;
        }
        return sum;
    }
    
    
     public int min(Operation op)
    {   
        int min = Integer.MAX_VALUE, nr;
        Table t = tables.get(tableName);
        int index = t.columnNames.get(op.colName);              //coloana la care se refera operatia
        for(int i=0;i<indices.size();i++)
        {
            nr = (int) (t.columns.get(index)).get(indices.get(i));
            if(nr < min)
                min = nr;
        }
        return min;
    }
     
      public int max(Operation op)
    {   
        int max = Integer.MIN_VALUE, nr;
        Table t = tables.get(tableName);
        int index = t.columnNames.get(op.colName);              //coloana la care se refera operatia
        for(int i=0;i<indices.size();i++)
        {
            nr = (int) (t.columns.get(index)).get(indices.get(i));
            if(nr > max)
                max = nr;
        }
        return max;
    }
      
      public int count(Operation op)
    {   
        int cnt = 0;
        Table t = tables.get(tableName);
        int index = t.columnNames.get(op.colName);              //coloana la care se refera operatia
        for(int i=0;i<indices.size();i++)
            cnt++;
        return cnt;
    }
      
   /*    public int avg(Operation op)
    {   
        int sum = 0, cnt = 0, nr;
        Table t = tables.get(tableName);
        int index = t.columnNames.get(op.colName);              //coloana la care se refera operatia
        for(int i=0;i<indices.size();i++)
        {
            nr = (int) (t.columns.get(index)).get(indices.get(i));
            sum+=nr;
            cnt++;
        }
        return sum/cnt;
    }
    */
      
}
