package cool.AST;

import java.util.ArrayList;
import java.util.Arrays;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;

public class LiteralNode implements ASTNode {
	ParserRuleContext context;
	Token token;
	String val, type;
	
	public LiteralNode(ParserRuleContext context, Token token, String type) {
		this.context = context;
		this.token = token;
		this.val = token.getText();
		this.type = type;
	}
	
	public Token getToken()
	{
		return token;
	}
	
	@Override
	public String toStr(String spaces) {
		return spaces + this.val + '\n';
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		ArrayList<String> dontCheck = new ArrayList<>(Arrays.asList("Int", "String", "Bool"));
		
		if(dontCheck.contains(type))
			return type;
		
		ArrayList<String> specialParents = new ArrayList<>(Arrays.asList("IO", "Int", "String", "Bool", "SELF_TYPE", "Object"));
		DefaultScope currentScope = attrScope;
		DefaultScope parScope = currentScope;
		boolean keepChecking = true;
		boolean foundDef = false;
		Symbol res;
		
		while(keepChecking)
		{
			if (specialParents.contains(parScope.getName()))
				keepChecking = false;
			else if( (res = parScope.lookup(val)) != null)
			{
				foundDef = true;
				this.type = res.getType();
				keepChecking = false;
			}
			currentScope = parScope;
			parScope = (DefaultScope) currentScope.getParent();
		}
		
		if (!foundDef)
		{
			String errStr = "Undefined identifier " + val;
			SymbolTable.error(context, token, errStr);
		}
		
		return type;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
