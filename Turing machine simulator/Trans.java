import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


public class Trans extends TM {						//Turing machine containing transitions
	HashMap<Character, Complex> transitions;

	public Trans(String name) {
		this.name = name;
		this.transitions = new HashMap<Character, Complex>();					//assigns a transition to a character
	}

	public void addTrans(Character c, Complex tm) {
		this.transitions.put(c, tm);
	}

	public void add(Complex tm) {			//updates all 
		String name = tm.name;

		Complex c;
		for (Map.Entry<Character, Complex> entry : this.transitions.entrySet()) {
			c = entry.getValue();
			if(name.equals(c.name))
				this.transitions.put(entry.getKey(), tm);
		}
	}

	public void replaceAliases() {				//replaces the alias of a symbol (alias@{symbol}) with the symbol itself
		Character key;
		Complex value;
		for (Map.Entry<Character, Complex> entry : this.transitions.entrySet()) {
			key = entry.getKey();
			value = entry.getValue();
			Complex aux = new Complex(value.name);
			aux.insideTM = (ArrayList<TM>) value.insideTM.clone();
			transitions.put(key,aux);
		}

		for (Map.Entry<Character, Complex> entry : this.transitions.entrySet()) {
			key = entry.getKey();
			value = entry.getValue();
			
			ArrayList<TM> arr = value.insideTM;
			if(!arr.isEmpty())
			for (int i = 0; i < arr.size(); i++) {
				TM tm = arr.get(i);
				if(tm.name.equals("|&"))
					arr.set(i,new ElementOne('H', key, "|"));
			}
		}
	}

	@Override
	public void run(ArrayList<Character> tape) {
		this.replaceAliases();
		int headInd = tape.indexOf('>');
		if(headInd + 1 == tape.size())
			tape.add('#');
		Complex mt = this.transitions.get(tape.get(headInd + 1));
		Character c = tape.get(headInd + 1);
		if(mt != null)
		{
			mt.run(tape);
		}
	}

	@Override
	public String toString() {
		String result = "( ";
		String delim = " ";

		for (Map.Entry<Character, Complex> entry : this.transitions.entrySet()) {
			result += "{" + entry.getKey() + "} -> " + entry.getValue();
			result += ";\n";
		}
		result += ")";
		return result;
	}
}