package cool.AST;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.structures.DefaultScope;
import cool.structures.SymbolTable;

public class NewOpNode implements ASTNode{
	String name;
	Token token;
	ParserRuleContext context;
	
	public NewOpNode(ParserRuleContext context, Token token) {
		this.context = context;
		this.token = token;
		this.name = token.getText();
	}
	
	@Override
	public String toStr(String spaces) {
		String toRet = spaces + "new\n";
		toRet += spaces + "  " + name + '\n';
		
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		if(SymbolTable.lookup(name) != null)
			return name;
		
		String errStr = "new is used with undefined type " + name;
		SymbolTable.error(context, token, errStr);

		return null;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
