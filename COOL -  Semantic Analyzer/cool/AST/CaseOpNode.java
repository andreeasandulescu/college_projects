package cool.AST;

import java.util.ArrayList;
import java.util.List;

import org.antlr.v4.runtime.ParserRuleContext;

import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;

public class CaseOpNode implements ASTNode{
	List<AttrNode> params = new ArrayList<AttrNode>();
	ASTNode expr;
	
	ParserRuleContext context;
	ArrayList<DefaultScope> localScopes;
	
	public CaseOpNode(ASTNode expr, ParserRuleContext context ) {
		this.context = context;
		this.expr = expr;
	}

	public void addParam(AttrNode a)
	{
		params.add(a);
	}
	
	public void CreateScope(DefaultScope parScope)
	{
		localScopes = new ArrayList<>();

		for(AttrNode param : params)
		{
			
			String errStr = "Case variable ";
			if(param.getName().equals("self"))
			{
				errStr = errStr + "has illegal name " + param.getName();
				SymbolTable.error(param.getContext(), param.getNameToken(), errStr);
			}
			else if(param.type.equals("SELF_TYPE"))
			{
				errStr = errStr + param.getName() + " has illegal type " + param.getType();
				SymbolTable.error(param.getContext(), param.getTypeToken(), errStr);
			}
			else if(!param.checkDefType())
			{
				errStr = errStr + param.getName() + " has undefined type " + param.getType();
				SymbolTable.error(param.getContext(), param.getTypeToken(), errStr);
			}
			
			DefaultScope scope = new DefaultScope("case", parScope);
			Symbol paramSymbol = new Symbol(param.getName(), param.getType(), param);
			scope.add(paramSymbol);
			localScopes.add(scope);
		}
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

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		CreateScope(attrScope);
		ArrayList<String> finalParamParents = null;
		
		for(int i = 0 ; i < params.size(); i++)
		{
			AttrNode param = params.get(i);
			DefaultScope paramScope = localScopes.get(i);
			ArrayList<String> paramParents = null;
			String paramType = null;
			
			if(paramScope != null)
				paramType = param.getReturnType(localScopes.get(i), methScope);

			if(paramType != null)
			{
				Symbol paramSymb = SymbolTable.lookup(paramType);
				if(paramSymb != null)
					paramParents = paramSymb.getParentList();
			}
			if(paramParents != null)
			{
				
				ArrayList<String> newParamParents= new ArrayList<>(paramParents);
				newParamParents.add(paramType);
				if(params.indexOf(param) == 0)
				{
					finalParamParents = newParamParents;
				}
				else
				{
					finalParamParents.retainAll(newParamParents);	
				}
			}
		}
		
		if(finalParamParents != null && finalParamParents.size() > 0)
			return finalParamParents.get(finalParamParents.size() - 1);
		
		return null;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
