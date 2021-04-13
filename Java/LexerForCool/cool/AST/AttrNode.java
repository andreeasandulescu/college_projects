package cool.AST;

public class AttrNode implements ASTNode {
	ASTNode expr = null;
	String name, type, op;
	
	public void setExpr(ASTNode expr)
	{
		this.expr = expr;
	}
	
	public AttrNode(String name, String type, int op) {
		this.name= name;
		this.type = type;
		
		switch(op)
		{
			case 1:
				this.op = "attribute";
				break;
			case 2:
				this.op = "local";
				break;
			case 3:
				this.op = "case branch";
				break;
			default:
				this.op = "undefined";
				break;
			
		}
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + op + '\n';
		
		toRet += newSpaces + name + '\n';
		toRet += newSpaces + type + '\n';
		if(expr != null)
			toRet += expr.toStr(newSpaces);
		return toRet;
	}

}
