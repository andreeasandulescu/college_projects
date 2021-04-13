package cool.AST;

import java.util.ArrayList;
import java.util.List;

public class CaseOpNode implements ASTNode{
	List<AttrNode> params = new ArrayList<AttrNode>();
	ASTNode expr;
	
	public CaseOpNode(ASTNode expr) {
		this.expr = expr;
	}

	public void addParam(AttrNode a)
	{
		params.add(a);
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "case\n";
		
		toRet += expr.toStr(newSpaces);
		
		for(AttrNode a : params)
			toRet += a.toStr(newSpaces);
			
		return toRet;
	}
}
