package cool.AST;

public class StrNode implements ASTNode {
	public String val;
		
	public StrNode(String str) {
		this.val = str;
	}
	
	@Override
	public String toStr(String spaces) {
		return spaces + this.val + '\n';
	}
	

}
