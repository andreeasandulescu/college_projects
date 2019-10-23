package cool.AST;

public class Formal {
	String name, type;
	
	public Formal(String name, String type) {
		this.name = name;
		this.type = type;
	}
	
	public String toStr(String spaces)
	{
		String newSpaces = spaces+ "  ";
		String toRet = spaces + "formal\n";
		
		toRet += newSpaces + name + '\n';
		toRet += newSpaces + type + '\n';
		return toRet;
	}
}