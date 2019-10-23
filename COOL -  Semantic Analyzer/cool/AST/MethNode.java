package cool.AST;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.parser.CoolParser.MethContext;
import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;

public class MethNode implements ASTNode{
	List<FormalNode> params = new ArrayList<FormalNode>();
	MethContext context;
	Token tName, tType;
	String name, type;
	ASTNode body;
	
	DefaultScope attrScope;
	DefaultScope methScope;
	
	public MethNode(MethContext context) {
		this.context = context;
		this.tName = context.name;
		this.tType = context.type;
		this.name = tName.getText();
		this.type = tType.getText();
	}
	
	public void setBody(ASTNode n)
	{
		body = n;
	}
	
	public void addParam(FormalNode f)
	{
		params.add(f);
	}
	
	public String getName()
	{
		return this.name;
	}
	
	public String getType()
	{
		return this.type;
	}
	
	public Token getNameToken()
	{
		return tName;
	}
	
	public Token getTypeToken()
	{
		return tType;
	}
	
	public int getFormalParamCnt()
	{
		return params.size();
	}
	
	public List<FormalNode> getParams()
	{
		return params;
	}
	
	public boolean checkDefType()
	{
		Symbol typeSymbol = SymbolTable.lookup(type);
		return (typeSymbol != null);
	}
	
	public void createScope(DefaultScope attrParentScope, DefaultScope methParentScope)
	{
		attrScope = new DefaultScope(name, attrParentScope);
		this.methScope = methParentScope;
		boolean methNodehasSemanticErrors = false;
		boolean succesfullyAdded = true;
		
		DefaultScope parScope = (DefaultScope)attrScope.getParent();		//parent class scope
		String parClass = parScope.getName();
		
		for(FormalNode param : params)
		{
			String errStr = "Method " + name + " of class " + parClass;
			
			if(param.getName().equals("self"))
			{
				errStr = errStr + " has formal parameter with illegal name " + param.getName();
				SymbolTable.error(context, param.getNameToken(), errStr);
				methNodehasSemanticErrors = true;
			}
			else if(param.getType().equals("SELF_TYPE"))
			{
				errStr = errStr + " has formal parameter " + param.getName() + " with illegal type " + param.getType();
				SymbolTable.error(context, param.getTypeToken(), errStr);
				methNodehasSemanticErrors = true;
			}
			else if(!param.checkDefType())
			{
				errStr = errStr + " has formal parameter " + param.getName()+ " with undefined type " + param.getType();
				SymbolTable.error(context, param.getTypeToken(), errStr);
				methNodehasSemanticErrors = true;
			}
			else
			{
				Symbol paramSymbol = new Symbol(param.getName(), param.getType(), param);
				succesfullyAdded = this.attrScope.add(paramSymbol);
				
				if(!succesfullyAdded)
				{
					errStr = errStr + " redefines formal parameter " + param.getName();
					SymbolTable.error(context, param.getNameToken(), errStr);
					methNodehasSemanticErrors = true;
				}
			}
		}
		
		if(!methNodehasSemanticErrors)
			checkOverriding();
	}
	
	public void checkOverriding()
	{
		boolean keepChecking = true;
		ArrayList<String> specialParents = new ArrayList<>(Arrays.asList("IO", "Int", "String", "Bool", "Object", "SELF_TYPE"));
	
		DefaultScope parentClass = (DefaultScope) methScope;
		DefaultScope currentScope = parentClass;
		
		
		if (specialParents.contains(currentScope.getName()))
			return; 
		
		while(keepChecking)
		{
			DefaultScope parScope = (DefaultScope) currentScope.getParent();
			Symbol inhMeth = parScope.lookup(name);	//the object corresponding to the inherited mehtod with the same name
			
			if (specialParents.contains(parScope.getName()))
				keepChecking = false;
			else if (inhMeth != null)
			{
				MethNode inhMethNode = (MethNode) inhMeth.getASTNode();
				String errStr = "Class " + parentClass.getName() + " overrides method " + name;
			
				if(getFormalParamCnt() != inhMethNode.getFormalParamCnt())
				{
					errStr = errStr + " with different number of formal parameters";
					SymbolTable.error(context, getNameToken(), errStr);
					keepChecking = false;		
				}
				else if(changesFormalParamType(inhMethNode, errStr) == true)
				{
					keepChecking = false;		
				}
				else if(!(inhMethNode.getType().equals(type)))
				{
					errStr = errStr + " but changes return type from " + inhMethNode.getType() + " to " + type;
					SymbolTable.error(context, tType, errStr);
					keepChecking = false;	
				}
			}
			currentScope = parScope;
		}
	}
	
	public boolean changesFormalParamType(MethNode toCmp, String auxErr)	//returns true for overrides with different param type
	{
		FormalNode p, r;
		String errStr;
		List<FormalNode> toCmpParams = toCmp.getParams();
		
		if(getFormalParamCnt() != toCmp.getFormalParamCnt())
			return false;
		
		int size = params.size();
		for(int i = 0 ; i < size; i++)
		{
			p = params.get(i);
			r = toCmpParams.get(i);
			if(p.cmpFormalsType(r))
			{
				errStr = auxErr + " but changes type of formal parameter " + p.getName();
				errStr = errStr + " from " + r.getType() + " to " + p.getType();
				SymbolTable.error(context, p.getTypeToken(), errStr);
				return true;
			}
		}
		return false;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "method\n";
		
		toRet += newSpaces + name + '\n';
		if(!params.isEmpty())
			for(FormalNode f : params)
				toRet += f.toStr(newSpaces);
		
		toRet += newSpaces + type + '\n';
		toRet += body.toStr(newSpaces);
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		String retType = body.getReturnType(this.attrScope, methScope);
		
	if(retType == null)
		return type;
		
	boolean check = SymbolTable.checkCompatTypes(type, retType, tType, body.getContext().start,  context);
	   
		if(!check)
		{
			String errStr = "Type " + retType + " of the body of method " + name ;
			errStr = errStr + " is incompatible with declared return type " + type;
			SymbolTable.error(context, body.getContext().start, errStr);
		}
		return type;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
