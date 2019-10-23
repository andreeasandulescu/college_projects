package cool.AST;

public class WhileNode implements ASTNode{
	ASTNode cond, expr;

	public void addOperands(ASTNode cond, ASTNode expr)
	{
		this.cond = cond;
		this.expr = expr;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "while\n";
		
		toRet += cond.toStr(newSpaces);
		toRet += expr.toStr(newSpaces);
		
		return toRet;
	}

}
