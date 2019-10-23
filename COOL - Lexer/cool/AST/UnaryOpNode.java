package cool.AST;

public class UnaryOpNode implements ASTNode{
	String operator;
	ASTNode operand;
	
	public UnaryOpNode(String operator) {
		this.operator = operator;
	}
	
	public void addOperand(ASTNode operand)
	{
		this.operand = operand;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = "";
		
		if(!operator.isEmpty())
		{
			toRet = spaces + operator + '\n';
			toRet += operand.toStr(newSpaces);
		}
		else
			toRet += operand.toStr(spaces);
		
		return toRet;
	}

}
