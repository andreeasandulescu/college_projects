package cool.AST;

import java.util.ArrayList;
import java.util.List;

public class ClassNode  implements ASTNode{
	
	String name, parName;								//parName is the name of the inherited class
	List<ASTNode> features = new ArrayList<ASTNode>();

	public ClassNode(String name) {
		this.name = name;
		this.parName = null;
	}
	
	public ClassNode(String name, String parName) {
		this.name = name;
		this.parName = parName;
	}
	
	public void addFeat(ASTNode f)
	{
		features.add(f);
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "class\n";
			
		toRet += newSpaces + name + '\n';
		if(parName != null)
			toRet += newSpaces + parName + '\n';
		
		if(!features.isEmpty())
			for(ASTNode node : features)
				toRet += node.toStr(newSpaces);
		
		return toRet;
	}
}



