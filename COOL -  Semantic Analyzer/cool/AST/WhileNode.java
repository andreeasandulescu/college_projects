package cool.AST;

import org.antlr.v4.runtime.ParserRuleContext;

import cool.structures.DefaultScope;
import cool.structures.SymbolTable;

public class WhileNode implements ASTNode{
	ASTNode cond, expr;
	ParserRuleContext context;

	public void addOperands(ParserRuleContext context, ASTNode cond, ASTNode expr)
	{
		this.context = context;
		this.cond = cond;
		this.expr = expr;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "while\n";
		
		toRet += cond.toStr(newSpaces);
		toRet += expr.toStr(newSpaces);
		
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		String res = cond.getReturnType(attrScope, methScope);
		if(!res.equals("Bool"))
		{
			String errStr = "While condition has type " + res + " instead of Bool";
			SymbolTable.error(context, cond.getContext().start, errStr);
		}
		return "Object";
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
