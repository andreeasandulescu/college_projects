package cool.AST;

import java.util.ArrayList;
import java.util.List;

import org.antlr.v4.runtime.ParserRuleContext;

import cool.structures.DefaultScope;
import cool.structures.Scope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;

public class ProgNode implements ASTNode{
	
	List<ClassNode> classes = new ArrayList<ClassNode>();

	public ProgNode()
	{
		SymbolTable.defineBasicClasses();
	}
	
	public void addChild(ClassNode c) {
		classes.add(c);
	}
	
	public List<ClassNode> getChildren()
	{
		return this.classes;
	}
	
	public ClassNode getChild(String name)
	{
		for(ClassNode node : classes)
		{
			if (node.getName().equals(name))
				return node;
		} 
		return null;
	}
	
	public Scope getScope() {
		return SymbolTable.globals;
	}
	
	public void populateScope()
	{
		boolean succesfullyAdded = false;
		for(ClassNode node : classes)
		{
			Symbol s = new Symbol(node.getName(), "", node);
			succesfullyAdded = SymbolTable.addSymbolToGlobalScope(s);
			
			if(!succesfullyAdded)
			{
				String errStr = "Class " + node.getName() + " is redefined";
				SymbolTable.error(node.getContext(), node.getNameToken(), errStr);
			}
		}
	}
	
	public void createChildrenScopes()
	{
		for(ClassNode node : classes)
			node.createScopes(getScope());	
	}
	
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = "program\n";
		
		for(ClassNode node : classes)
			toRet += node.toStr(newSpaces);
		
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		
		for (ClassNode node : classes)
			node.getReturnType(attrScope, methScope);

		return "Object";
	}

	@Override
	public ParserRuleContext getContext() {
		return null;
	}
}
