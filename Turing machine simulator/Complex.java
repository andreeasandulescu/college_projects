import java.util.ArrayList;

public class Complex extends TM{			//Turing Machine containing more Turing Machines
	ArrayList<TM> insideTM;

	public Complex(String name) {
		this.name = name;
		this.insideTM = new ArrayList<TM> ();
	}

	public void add(TM tm)
	{
		this.insideTM.add(tm);
	}

	@Override
	public void run(ArrayList<Character> tape) {
		for(TM tm:this.insideTM)
			tm.run(tape);
	}

	public String arrToStr(ArrayList<Character> tape)
	{
		StringBuilder result = new StringBuilder(tape.size());
		for (Character c : tape) {
		  result.append(c);
		}
		return result.toString();
	}

	@Override
	public String toString() {
		String result = "[";
		String delim = ",";


		for (TM mt : insideTM) {
			result += delim + mt.toString();
		}
		return result + "]";
	}
}
