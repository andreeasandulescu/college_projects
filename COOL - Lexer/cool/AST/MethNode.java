package cool.AST;

import java.util.ArrayList;
import java.util.List;

public class MethNode implements ASTNode{
	List<Formal> params = new ArrayList<Formal>();
	String name, type;
	ASTNode body;
	
	public MethNode(String name, String type) {
		this.name = name;
		this.type = type;
	}
	
	public void setBody(ASTNode n)
	{
		body = n;
	}
	
	public void addParam(Formal f)
	{
		params.add(f);
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "method\n";
		
		toRet += newSpaces + name + '\n';
		if(!params.isEmpty())
			for(Formal f : params)
				toRet += f.toStr(newSpaces);
		
		toRet += newSpaces + type + '\n';
		toRet += body.toStr(newSpaces);
		return toRet;
	}

}
