package cool.AST;

public class IfNode implements ASTNode{
	ASTNode ifExpr, thenExpr, elseExpr;
	
	public void addExpressions(ASTNode ifExpr, ASTNode thenExpr, ASTNode elseExpr)
	{
		this.ifExpr = ifExpr;
		this.thenExpr = thenExpr;
		this.elseExpr = elseExpr;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "if\n";
		
		toRet += ifExpr.toStr(newSpaces);
		toRet += thenExpr.toStr(newSpaces);
		toRet += elseExpr.toStr(newSpaces);
		
		return toRet;
	}

}
