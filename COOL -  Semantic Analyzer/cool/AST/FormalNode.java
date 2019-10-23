package cool.AST;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.parser.CoolParser.FormalContext;
import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;


public class FormalNode implements ASTNode{
	String name, type;
	Token tName, tType;
	FormalContext context;
	
	public FormalNode(FormalContext context) {
		this.context = context;
		this.tName = context.name;
		this.tType = context.type;
		this.name = tName.getText();
		this.type = tType.getText();
	}
	
	public String getName()
	{
		return name;
	}
	
	public String getType()
	{
		return type;
	}
	
	public Token getNameToken()
	{
		return tName;
	}
	
	public Token getTypeToken()
	{
		return tType;
	}
	
	public boolean cmpFormalsType(FormalNode toCmp)		//returns true for object with different types
	{
		if(!type.equals(toCmp.getType()))
			return true;
		return false;
	}
	
	public boolean checkDefType()
	{
		Symbol typeSymbol = SymbolTable.lookup(type);
		return (typeSymbol != null);
	}
	
	
	public String toStr(String spaces)
	{
		String newSpaces = spaces+ "  ";
		String toRet = spaces + "formal\n";
		
		toRet += newSpaces + name + '\n';
		toRet += newSpaces + type + '\n';
		return toRet;
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