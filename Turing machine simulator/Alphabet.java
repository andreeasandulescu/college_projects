
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Arrays;

public class Alphabet {
	HashMap<String, Character> symbols;
	HashMap<String, ArrayList<Character> > symbolSet;
	
	public Alphabet() {
		this.symbols = new HashMap<String, Character>();						//for symbol definitions
		this.symbolSet = new HashMap<String, ArrayList<Character> >();			//for symbol sets definitions
	}
	
	public ArrayList<Character> getByName(String str) {
		ArrayList <Character> result = new ArrayList<Character> ();
		Character c = symbols.get(str);
		if(c != null)
		{
			result.add(c);
			return result;
		}
		return symbolSet.get(str);
	}

	public void addSymbol(char c) {
		this.symbols.put("~" + c, c);
	}

	public void addSymbol(char c, String str) {
		if(this.contains(c))
			this.symbols.remove("~" + c);
		this.symbols.put(str, c);
	}
	
	public boolean contains(char c) {
		return this.symbols.containsKey("~" + c);
	}

	public boolean contains(String str) {
		return this.symbols.containsKey(str);
	}

	public void addSet(String set) {
		
		this.symbolSet.put(set, new ArrayList<Character>());
	}

	public boolean containsSet(String set)
	{
		return this.symbolSet.containsKey(set);
	}

	public void addSymbolToSet(String set, char c) {
		ArrayList<Character> list = this.symbolSet.get(set);
		list.add(c);
	}


	public boolean setContainsSymbol(String set, char c)
	{
		ArrayList<Character> list = this.symbolSet.get(set);
		return list.contains(c);
	}
	
	@Override
	public String toString() {
		String result = "{";
		String delim = "";
		for(String str:this.symbols.keySet()){
			result += delim + str;
			delim = ",";
		}
		result += "}";
		return result;
	}

	public String setsToString() {
		String result = "{";
		String delim = "";

		for (Map.Entry<String, ArrayList<Character>> entry : this.symbolSet.entrySet()) {
	    String key = entry.getKey();
	    ArrayList<Character> list = entry.getValue();
	    String str = Arrays.toString(list.toArray());
	    result += delim + "[" + key + "]: " + str;
		}

		result += "}";
		return result;

	}
}
