package cool.AST;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.structures.DefaultScope;

public class StrNode implements ASTNode {
	ParserRuleContext context;
	Token token;
	String val, type;
		
	public StrNode(ParserRuleContext context, Token token, String type) {
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
		return type;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}
	

}
