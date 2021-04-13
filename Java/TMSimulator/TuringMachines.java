import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

public class TuringMachines {
	HashMap<String, TM> fileTM;		//all the Turing Machines in the file
	Stack<TM> stack;
	ArrayList<Character> tape;
	
	public TuringMachines() {
		this.fileTM = new HashMap<String, TM>();
		this.stack = new Stack<TM>();
		this.tape = new ArrayList<Character>();
	}

	public void push(TM tm) {
		this.stack.push(tm);
	}

	public TM pop() {
		return this.stack.pop();
	}
	
	public void addFinalTM() {
		this.stack = this.stackReverse();
		Complex tm = (Complex) this.stack.pop();
		while(this.stack.size() > 0)
		{
			TM aux = this.stack.pop();
			tm.add(aux);
		}
		this.fileTM.put(tm.name, tm);
	}

	public Stack<TM> stackReverse()
	{
		Stack<TM> aux = new Stack<TM>();
		while(this.stack.size() > 0)
		{
			aux.push(this.stack.pop());
		}
		return aux;
	}

	public void readTape(String str)
	{
		for (char c : str.toCharArray())
  			tape.add(c);
	}

	public String arrToStr()
	{
		StringBuilder result = new StringBuilder(this.tape.size());
		for (Character c : this.tape) {
		  result.append(c);
		}
		return result.toString();
	}

	public void runMT(String mtName, String str)
	{
		this.readTape(str);
		Complex toRun = (Complex) this.fileTM.get(mtName);
		for(TM tm: toRun.insideTM)
		{
			tm.run(tape);
		}
		this.cleanTape();
		System.out.println(this.arrToStr());
		
	}

	public void cleanTape(){					//clears the redundant "#", or adds "#", if necessary
		int headInd = tape.indexOf('>');
		int size;
		Character fin, start;

		while(headInd > 1)
		{
			tape.remove(0);
			headInd = tape.indexOf('>');
		}
		size = tape.size();
		fin = tape.get(size - 1);
		if(!fin.equals('#'))
			tape.add('#');
		else while(fin.equals(tape.get(size - 2)))
		{
			tape.remove(size - 1);
			size--;
		}
		start = tape.get(0);
		if(!start.equals('#'))
			tape.add(0,'#');
	}

	@Override
	public String toString() {
		String result = "{";
		String delim = " ";
		Complex tm;
		for(Map.Entry<String, TM> entry : this.fileTM.entrySet())
		{
			tm = (Complex) entry.getValue();
			result += "[" + tm.name + "]= ";
			for(TM aux:tm.insideTM){
			result += aux.toString();
			result += delim;
			}
		}

		result += "}";
		return result;
	}
	
	
}
