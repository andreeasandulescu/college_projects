package cool.AST;

public class BinaryOpNode implements ASTNode {
	ASTNode left, right;
	String operator;
	
	public BinaryOpNode(String operator) {
		this.operator = operator;
	}

	public void addOperands(ASTNode left, ASTNode right)
	{
		this.left = left;
		this.right = right;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + operator + '\n';
		
		toRet += left.toStr(newSpaces);
		toRet += right.toStr(newSpaces);
		
		return toRet;
	}

}
