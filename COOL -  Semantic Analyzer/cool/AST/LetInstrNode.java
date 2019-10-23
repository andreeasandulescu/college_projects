package cool.AST;

import java.util.ArrayList;
import java.util.List;

import org.antlr.v4.runtime.ParserRuleContext;

import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;

public class LetInstrNode implements ASTNode{
	ASTNode expr;
	List<AttrNode> params = new ArrayList<AttrNode>();
	DefaultScope localScope, methScope;
	ParserRuleContext context;
	
	public LetInstrNode(ParserRuleContext context, ASTNode expr) {
		this.context = context;
		this.expr = expr;
	}
	
	public void addParam(AttrNode a)
	{
		params.add(a);
	}
	
	public void CreateScope(DefaultScope parScope)
	{
		localScope = new DefaultScope("let", parScope);
		
		for(AttrNode param : params)
		{
			param.getReturnType(localScope, methScope);
			String errStr = "Let variable ";
			if(param.getName().equals("self"))
			{
				errStr = errStr + "has illegal name " + param.getName();
				SymbolTable.error(param.getContext(), param.getNameToken(), errStr);
			}
			else
			{
				if(!param.checkDefType())
				{
					errStr = errStr + param.getName() + " has undefined type " + param.getType();
					SymbolTable.error(param.getContext(), param.getTypeToken(), errStr);
				}
				
				Symbol paramSymbol = new Symbol(param.getName(), param.getType(), param);
				localScope.add(paramSymbol);
			}
		}
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

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		this.methScope = methScope;
		CreateScope(attrScope);
		return expr.getReturnType(localScope, methScope);
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
