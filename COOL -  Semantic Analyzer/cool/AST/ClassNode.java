package cool.AST;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.antlr.v4.runtime.Token;

import cool.parser.CoolParser.Class_defContext;
import cool.structures.SymbolTable;
import cool.structures.DefaultScope;
import cool.structures.Scope;
import cool.structures.Symbol;

public class ClassNode  implements ASTNode{
	
	Class_defContext context;
	Token tName, tParName;
	String name, parName;
	List<ASTNode> features = new ArrayList<ASTNode>();
	
	List<AttrNode> attributes = new ArrayList<AttrNode>();
	List<MethNode> methods = new ArrayList<MethNode>();
	
	DefaultScope attribScope;
	DefaultScope methScope;
	
	public static String checkName(Class_defContext context)	//returns error string if class name is not valid
	{
		String name = context.name.getText();
		
		if(name.equals("SELF_TYPE"))
			return "Class has illegal name SELF_TYPE";
		return null;
	}

	public ClassNode(Class_defContext context) {
		this.context = context;
		this.tName = context.name;
		this.name = tName.getText();
		if (context.parName != null)
		{
			this.tParName = context.parName;
			this.parName = tParName.getText();
		}
		else
		{
			this.tParName = null;
			this.parName = null;
		}
		
	}	
	
	public void addFeat(ASTNode f)
	{
		features.add(f);
	}
	
	public void addAttr(AttrNode a)
	{
		attributes.add(a);
	}
	
	public void addMeth(MethNode m)
	{
		methods.add(m);
	}
	
	public boolean hasParentClass()
	{
		return (tParName != null);
	}
	public String getName()
	{
		return this.name;
	}
	
	public Token getNameToken()
	{
		return this.tName;
	}
	
	public Token getParToken()
	{
		return this.tParName;
	}
	
	public Scope getMethScope()
	{
		return this.methScope;
	}
	
	public Scope getAttrScope()
	{
		return this.attribScope;
	}
	
	public void setParAttrScope(Scope parAttr)
	{
		attribScope.setParent(parAttr);
	}
	
	public void setParMethScope(Scope parMeth)
	{
		methScope.setParent(parMeth);
	}
	
	public Class_defContext getContext()
	{
		return this.context;
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
	
	public void createScopes(Scope parentScope)
	{
		this.attribScope = new DefaultScope(name, parentScope);
		this.methScope = new DefaultScope(name, parentScope);
		boolean succesfullyAdded = false;
		
		Symbol selfSymb = new Symbol("self", name, null);
		attribScope.add(selfSymb);
		
		for (AttrNode node : this.attributes)
		{
			if(node.getName().equals("self"))
			{
				String errStr = "Class " + name + " has attribute with illegal name self";
				SymbolTable.error(context, node.getNameToken(), errStr);
			}
			else
			{
				Symbol s = new Symbol(node.getName(), node.getType(), node);
				succesfullyAdded = this.attribScope.add(s);
				
				if(!succesfullyAdded)
				{
					String errStr = "Class " + name + " redefines attribute " + node.getName();
					SymbolTable.error(context, node.getNameToken(), errStr);
				}
				else if(node.checkDefType() == false)
				{
					String type = node.getTypeToken().getText();
					String errStr = "Class " + name + " has attribute " + node.getName() + " with undefined type " + type;
					SymbolTable.error(context, node.getTypeToken(), errStr);
				}
			}
		}
		
		for (MethNode node : this.methods)
		{
			Symbol s = new Symbol(node.getName(), node.getType(), node);
			succesfullyAdded = this.methScope.add(s);
			
			if(!succesfullyAdded)
			{
				String errStr = "Class " + name + " redefines method " + node.getName();
				SymbolTable.error(context, node.getNameToken(), errStr);
			}
			else if(node.checkDefType() == false)
			{
				String type = node.getTypeToken().getText();
				String errStr = "Class " + name + " has method " + node.getName() + " with undefined return type " + type;
				SymbolTable.error(context, node.getTypeToken(), errStr);
			}
		}
	}
	
	public void checkParentScope() 
	{
		if(tParName == null)
		{
			Symbol type = SymbolTable.lookup(name);
			if(type != null)
			{
				ArrayList<String> parList = new ArrayList<String>();
				parList.add("Object");
				
				type.setParentList(parList);
			}
			return;
		}
		
		
		ArrayList<String> illegalParents = new ArrayList<>(Arrays.asList("Int", "String", "Bool", "SELF_TYPE"));
		if(illegalParents.contains(parName))
		{
			String errStr = "Class " + name + " has illegal parent " + parName;
			SymbolTable.error(context, tParName, errStr);
			return;
		}
		
		Symbol parentSymbol = SymbolTable.lookup(parName);
		
		if(parentSymbol == null)
		{
			String errStr = "Class " + name + " has undefined parent " + parName;
			SymbolTable.error(context, tParName, errStr);
			return;
		}
		
		boolean keepChecking = true;
		DefaultScope currentScope = attribScope;
		ArrayList<String> parList = new ArrayList<String>();
		
		while(keepChecking)
		{
			DefaultScope parScope = (DefaultScope) currentScope.getParent();
			if (parScope.getName().equals(name))
			{
				String errStr = "Inheritance cycle for class " + name;
				SymbolTable.error(context, tName, errStr);
				keepChecking = false;
			}
			
			if (parScope.getName().equals("Object"))
				keepChecking = false;
			
			parList.add(0, parScope.getName());
			currentScope = parScope;
		}
		
		Symbol type = SymbolTable.lookup(name);
		if(type != null)
			type.setParentList(parList);
	}
	
	public void checkAttributes()
	{
		ArrayList<String> specialParents = new ArrayList<>(Arrays.asList("IO", "Int", "String", "Bool", "SELF_TYPE", "Object"));
		DefaultScope currentScope = attribScope;
		boolean keepChecking = true;
		
		for(AttrNode node : attributes)
		{
			currentScope = attribScope;
			keepChecking = true;
			
			if(!hasParentClass())
				return;
			
			while(keepChecking)
			{
				DefaultScope parScope = (DefaultScope) currentScope.getParent();
				
				if (specialParents.contains(parScope.getName()))
					keepChecking = false;
				else if (parScope.lookup(node.getName()) != null)
				{
					String errStr = "Class " + name + " redefines inherited attribute " + node.getName();
					SymbolTable.error(context, node.getNameToken(), errStr);
					keepChecking = false;
				}
				currentScope = parScope;
			}
		}
	}
	
	public void createChildrenMethScopes()
	{
		for(MethNode node : methods)
			node.createScope(this.attribScope, this.methScope);
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		
		for(AttrNode aNode : attributes)
			aNode.getReturnType(this.attribScope, this.methScope);
		
		for(MethNode mNode : methods)
			mNode.getReturnType(this.attribScope, this.methScope);
		
		return name;
	}
}



