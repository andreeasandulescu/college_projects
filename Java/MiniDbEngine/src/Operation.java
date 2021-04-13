/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Andreea
 */
public class Operation {
    String op;
    String colName;

    public Operation(String operation) {
        if(operation.contains("(") && operation.contains(")"))
        {
            String[] aux1 = operation.split("\\(");
            String[] aux2 = aux1[1].split("\\)");
            op = aux1[0];
            colName = aux2[0];
        }
        else
        {
            op = "none";
            colName = operation;
        }
    }

    public void validOp(String columnType)
    {
        if(op.equals("none"))
            return;
        if(op.equals("count"))
            return;
        boolean check = op.equals("min") || op.equals("max") || op.equals("sum") || op.equals("avg");
        if(check && columnType.equals("int"))
            return;
        throw new RuntimeException("Operatie invalida!");
          
    }




}
