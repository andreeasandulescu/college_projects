package cool.AST;

import java.util.ArrayList;
import java.util.List;

import org.antlr.v4.runtime.ParserRuleContext;
import cool.structures.DefaultScope;

public class ChildrenListNode implements ASTNode{
	List<ASTNode> children = new ArrayList<>();
	String opName;
	ASTNode className, actualCName, methName;
	ParserRuleContext context;
	
	public ChildrenListNode(ParserRuleContext context, String opName, ASTNode className,ASTNode actualCName, ASTNode methName)
	{
		this.context = context;
		this.opName = opName;
		this.className = className;
		this.actualCName = actualCName;
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
		
		if(className != null)
			toRet += className.toStr(newSpaces); 
		
		if(actualCName != null)
			toRet += actualCName.toStr(newSpaces); 
			
		if(methName != null)
			toRet += methName.toStr(newSpaces); 
		
		if(!children.isEmpty())
			for(ASTNode a : children)
				toRet += a.toStr(newSpaces);
		
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		if(opName.equals("block"))
		{
			int size = children.size();
			if(size > 0)
				return children.get(size - 1).getReturnType(attrScope, methScope);
		}
		for(ASTNode node : children)
			node.getReturnType(attrScope, methScope);
		return null;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}


}
