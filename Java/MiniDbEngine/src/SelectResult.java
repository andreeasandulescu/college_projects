
import java.util.ArrayList;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Andreea
 */
public class SelectResult implements Callable <ArrayList<ArrayList<Object>>> {
    int indicesCnt;
    ArrayList<Future<ArrayList<ArrayList<Object>>>> arr;
    Operation[] op;

    public SelectResult(ArrayList<Future<ArrayList<ArrayList<Object>>>> arr, Operation[] op,int indicesCnt) {
        this.indicesCnt = indicesCnt;
        this.arr = arr;
        this.op = op;
    }
             
    
    @Override
    public ArrayList<ArrayList<Object>> call() throws Exception {
        ArrayList <ArrayList <ArrayList <Object>>> all = new ArrayList<>();
        ArrayList <ArrayList <Object>> aux;
        int nr1, nr2;
        
        if(arr.size() == 1)
        {
            try {
                aux = (ArrayList<ArrayList<Object>>) arr.get(0).get();
                for(int i = 0; i< aux.size();i++)
                {
                    if(op[i].op.equals("avg"))
                    {
                        nr1 = (int) aux.get(i).get(0);
                        aux.get(i).set(0, nr1/indicesCnt);
                    }
                }
            } catch (InterruptedException | ExecutionException ex) {
               throw new RuntimeException("Eroare in getFinalRes!");
            }
            return aux;
        }
        
        for(int i = 0; i < arr.size();i++)
        {
            try {
                all.add((ArrayList<ArrayList<Object>>) arr.get(i).get());
            } catch (InterruptedException | ExecutionException ex) {
               ex.printStackTrace();
               throw new RuntimeException("Eroare in getFinalRes!");
            }
        }

        for(int i =0;i<op.length;i++)
        {
            switch(op[i].op)
            {
            case "min":
                nr1 = (int) all.get(0).get(i).get(0);
                for(int j = 1; j < all.size();j++)
                {
                    nr2 = (int) all.get(j).get(i).get(0);
                    if(nr2 < nr1)
                         nr1 = nr2;
                }
                all.get(0).get(i).set(0, nr1);
                break;
            case "max":
                nr1 = (int) all.get(0).get(i).get(0);
                for(int j = 1; j < all.size();j++)
                {
                    nr2 = (int) all.get(j).get(i).get(0);
                    if(nr2 > nr1)
                        nr1 = nr2;
                }
                all.get(0).get(i).set(0, nr1);
                break;
            case "sum":
                nr1 = (int) all.get(0).get(i).get(0);
                for(int j = 1; j < all.size();j++)
                {
                    nr2 = (int) all.get(j).get(i).get(0);
                    nr1 += nr2;
                }
                all.get(0).get(i).set(0, nr1);
                break;
            case "avg":
                nr1 = (int) all.get(0).get(i).get(0);
                for(int j = 1; j < all.size();j++)
                {
                    nr2 = (int) all.get(j).get(i).get(0);
                    nr1 += nr2;
                }
                all.get(0).get(i).set(0, nr1 / indicesCnt);
                break;
            case "none":
                for(int j = 1; j < all.size();j++)
                  all.get(0).get(i).addAll(all.get(j).get(i));
                 break;
            }
        }
        return all.get(0);
        
    }
    
}
