package cool.AST;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.structures.DefaultScope;
import cool.structures.SymbolTable;

public class UnaryOpNode implements ASTNode{
	String operator;
	ASTNode operand;
	
	ParserRuleContext context;
	Token token;
	
	public UnaryOpNode(ParserRuleContext context, Token token) {
		this.context = context;
		this.token = token;
		this.operator = token.getText();
	}
	
	public void addOperand(ASTNode operand)
	{
		this.operand = operand;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = "";
		
		if(!operator.isEmpty())
		{
			toRet = spaces + operator + '\n';
			toRet += operand.toStr(newSpaces);
		}
		else
			toRet += operand.toStr(spaces);
		
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		String res = operand.getReturnType(attrScope, methScope);
		boolean resIsNotNull = res != null;
		
		if(operator.equals("isvoid"))
			return "Bool";
		
		if(resIsNotNull)
		{
			if(operator.equals("~"))
			{
				if(!res.equals("Int"))
				{
					String errStr = "Operand of " + operator + " has type " + res + " instead of Int";
					SymbolTable.error(context, operand.getContext().start, errStr);
					return null;
				}
				else
					return "Int";
			}
			
			if(operator.equals("not"))
			{
				if(!res.equals("Bool"))
				{
					String errStr = "Operand of " + operator + " has type " + res + " instead of Bool";
					SymbolTable.error(context, operand.getContext().start, errStr);
					return null;
				}
				else
					return "Bool";
			}
		}
		return res;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
