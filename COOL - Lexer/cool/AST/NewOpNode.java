package cool.AST;

public class NewOpNode implements ASTNode{
	String name;
	
	public NewOpNode(String name) {
		this.name = name;
	}
	
	@Override
	public String toStr(String spaces) {
		String toRet = spaces + "new\n";
		toRet += spaces + "  " + name + '\n';
		
		return toRet;
	}

}
