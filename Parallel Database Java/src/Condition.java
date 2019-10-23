/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Andreea
 */
public class Condition {

    String columnName;
    String comparator;
    Object value;
    String valueType;
    
    public Condition(String cond) {
        String[] aux = cond.split(" ");
        if(aux[0].isEmpty())
        {
            valueType = "any";
        }
        else
        {
            columnName = aux[0];
            comparator = aux[1];
            value = aux[2];
            if(Condition.isInteger((String)value))
            {
                valueType = "int";
                value = Integer.parseInt((String)value);
            }
            else if(Condition.isBoolean((String)value))
            {
                valueType= "bool";
                value = Boolean.valueOf((String)value);
            }
            else
                valueType = "string";
        }
    }
    
    public static boolean isInteger(String str) {
    int size = str.length();
    int aux=0;
    if(str.startsWith("-"))
        aux = 1;
    for (int i = aux; i < size; i++) {
        if (!Character.isDigit(str.charAt(i))) {
            return false;
        }
    }
    return size > 0;
    }
    
   public static boolean isBoolean(String str) {
       return (str.equals("true") || str.equals("false"));
    }

   void validCond(String columnType)
   {
     if(valueType.equals("any"))
     {
         return;
     }
     if(!valueType.equals(columnType))
         throw new RuntimeException("columnType != valueType");
      if(comparator.equals("<") && !valueType.equals("int"))
         throw new RuntimeException("< nu se poate aplica decat pe int");
      if(comparator.equals(">") && !valueType.equals("int"))
         throw new RuntimeException("> nu se poate aplica decat pe int");
   }
   
   boolean respectsCond(Object columnElem)
   {
       switch(valueType){
           case "any":
               return true;
           case "int":
               int nr = (int) columnElem;
               int val = (int) value;
               if((comparator.equals("==")) && (val == nr))
                   return true;
               if((comparator.equals("<")) && (nr < val))
                   return true;
               if((comparator.equals(">")) && (nr > val))
                   return true;
              break;
            case "string":
                String elem = (String) columnElem;
                return (elem.equals(value));
            case "bool":
                boolean b = (Boolean) columnElem;
                boolean bval = (Boolean) value;
                return (b == bval);
       }
       return false;
   }
  
    
}
