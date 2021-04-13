package cool.AST;

import java.util.ArrayList;
import java.util.List;

public class LetInstrNode implements ASTNode{
	ASTNode expr;
	List<AttrNode> params = new ArrayList<AttrNode>();
	
	public LetInstrNode(ASTNode expr) {
		this.expr = expr;
	}
	
	public void addParam(AttrNode a)
	{
		params.add(a);
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "let\n";
		
		for(AttrNode a : params)
			toRet += a.toStr(newSpaces);
		
		toRet += expr.toStr(newSpaces);
		
		return toRet;
	}

}
