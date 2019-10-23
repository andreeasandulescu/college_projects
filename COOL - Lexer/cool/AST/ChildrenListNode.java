package cool.AST;

import java.util.ArrayList;
import java.util.List;

public class ChildrenListNode implements ASTNode{
	List<ASTNode> children = new ArrayList<>();
	String opName;
	ASTNode objName, className, methName;
	
	public ChildrenListNode(String opName, ASTNode className,ASTNode actualCName, ASTNode methName) {
		this.opName = opName;
		this.objName = className;
		this.className = actualCName;
		this.methName = methName;
	}
	
	public void addChild(ASTNode node)
	{
		this.children.add(node);
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + opName + '\n';
		
		if(objName != null)
			toRet += objName.toStr(newSpaces); 
		
		if(className != null)
			toRet += className.toStr(newSpaces); 
			
		if(methName != null)
			toRet += methName.toStr(newSpaces); 
		
		if(!children.isEmpty())
			for(ASTNode a : children)
				toRet += a.toStr(newSpaces);
		
		return toRet;
	}

}
