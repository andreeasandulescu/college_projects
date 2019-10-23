package cool.AST;

public class LiteralNode implements ASTNode {
	String val;
	
	public LiteralNode(String val) {
		this.val = val;
	}
	
	@Override
	public String toStr(String spaces) {
		return spaces + this.val + '\n';
	}

}
