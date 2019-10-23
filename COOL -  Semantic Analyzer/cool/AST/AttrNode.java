package cool.AST;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.parser.CoolParser.AttrContext;
import cool.parser.CoolParser.AttribContext;
import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;
import cool.parser.CoolParser.Case_attribContext;

public class AttrNode implements ASTNode {
	
	ParserRuleContext context;
	Token tName, tType;
	ASTNode expr = null;
	String name, type, op;
	
	public AttrNode(ParserRuleContext context, int op) {
		this.context = context;
		
		if(context instanceof AttrContext)
		{
			AttrContext auxCtx = (AttrContext) context;
			this.tName = auxCtx.name;
			this.tType = auxCtx.type;
		}
		
		if(context instanceof AttribContext)
		{
			AttribContext auxCtx = (AttribContext) context;
			this.tName = auxCtx.name;
			this.tType = auxCtx.type;
		}
		
		if(context instanceof Case_attribContext)
		{
			Case_attribContext auxCtx = (Case_attribContext) context;
			this.tName = auxCtx.name;
			this.tType = auxCtx.type;
		}
		
		this.name = tName.getText();				//get the attribute name
		if(this.tType != null)						//check if the attribute has a defined type
			this.type = tType.getText();			 
		
		switch(op)
		{
			case 1:
				this.op = "attribute";
				break;
			case 2:
				this.op = "local";
				break;
			case 3:
				this.op = "case branch";
				break;
			default:
				this.op = "error_attribute";
				break;
			
		}
	}
	
	public void setExpr(ASTNode expr)
	{
		this.expr = expr;
	}
	
	public String getName()
	{
		return this.name;
	}
	
	public String getType()
	{
		return type;
	}
	
	public ParserRuleContext getContext()
	{
		return context;
	}
	
	public Token getNameToken()
	{
		return tName;
	}
	
	public Token getTypeToken()
	{
		return tType;
	}
	
	public boolean checkDefType()
	{
		Symbol typeSymbol = SymbolTable.lookup(type);
		return (typeSymbol != null);
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + op + '\n';
		
		toRet += newSpaces + name + '\n';
		toRet += newSpaces + type + '\n';
		if(expr != null)
			toRet += expr.toStr(newSpaces);
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		if(this.op.equals("case branch"))
		{
			if(expr != null)
			{
				String auxType = expr.getReturnType(attrScope, methScope);
				if(SymbolTable.lookup(auxType) != null)
					return auxType;
				else
					return null;
			}
		}
		
		if(this.op.equals("local") || this.op.equals("attribute"))
		{
			Symbol aux = SymbolTable.lookup(type);
			if(aux == null)
				return null;
			
			if(expr != null)
			{
				String rightType = expr.getReturnType(attrScope, methScope);
				if(rightType != null)
				{
					if(rightType.equals(type))
						return type;
					
					boolean check = SymbolTable.checkCompatTypes(type, rightType, tType, expr.getContext().start,  context);
					if(!check)
					{
						String errStr = "Type " + rightType + " of initialization expression of ";
						if(this.op.equals("local"))
							errStr = errStr + "identifier " + name;
						if(this.op.equals("attribute"))
							errStr = errStr + "attribute " + name;
						errStr = errStr + " is incompatible with declared type " + type;
						SymbolTable.error(context, expr.getContext().start, errStr);
					}
					return null;
				}
			}
		}
		
		
		if(this.tType != null)						
			return type;		
		return null;
	}

}
