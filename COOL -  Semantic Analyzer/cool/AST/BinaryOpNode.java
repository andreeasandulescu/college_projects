package cool.AST;

import java.util.ArrayList;
import java.util.Arrays;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;

public class BinaryOpNode implements ASTNode {
	ASTNode left, right;
	String operator;
	
	ParserRuleContext context;
	Token token, leftToken, rightToken;
	
	public BinaryOpNode(ParserRuleContext context, Token token, Token leftToken, Token rightToken) {
		this.context = context;
		this.token = token;
		this.leftToken = leftToken;
		this.rightToken = rightToken;
		this.operator = token.getText();
	}

	public void addOperands(ASTNode left, ASTNode right)
	{
		this.left = left;
		this.right = right;
	}
	
	public String checkType(String leftType, String rightType)
	{
		ArrayList<String> intOp = new ArrayList<>(Arrays.asList("+", "-", "*", "/", "<" , "<="));
		
		if(intOp.contains(operator))
		{
			String errStr = "Operand of " + operator + " has type " ;
			
			if(leftType != null && !leftType.equals("Int"))
			{
				errStr = errStr + leftType + " instead of Int";
				SymbolTable.error(context, leftToken, errStr);
			}	
			else if(rightType != null && !rightType.equals("Int"))
			{
				errStr = errStr + rightType + " instead of Int";
				SymbolTable.error(context, rightToken, errStr);
			}
			else
			{
				if(operator.equals("<") || operator.equals("<="))
						return "Bool";
				else
					return "Int";
			}
		}
		if(operator.equals("="))
		{
			ArrayList<String> cmpTypes = new ArrayList<>(Arrays.asList("Int", "String", "Bool"));
			if(cmpTypes.contains(leftType) || cmpTypes.contains(leftType))
				if(!leftType.equals(rightType))
				{
					String errStr = "Cannot compare " + leftType + " with " + rightType ;
					SymbolTable.error(context, token, errStr);
				}
		}
		if(operator.equals("<-"))
		{
			if(leftToken.getText().equals("self"))
			{
				String errStr = "Cannot assign to self" ;
				SymbolTable.error(context, leftToken, errStr);
			}
			else 
			{
				boolean compatibleType = leftType.equals(rightType);
				Symbol rightTypeSymbol = SymbolTable.lookup(rightType);
				
				if(rightTypeSymbol != null)
				{
					ArrayList<String> rightTypeParents = rightTypeSymbol.getParentList();
					
					if(rightTypeParents != null)
					{
						//check parent list for all classes except for Object which will have the parent list set to null
						compatibleType = compatibleType || rightTypeParents.contains(leftType);	
					}
				}
				else
					compatibleType = true;	//can't print 2 errors for same token
				
				if(!compatibleType)
				{
					String errStr = "Type " + rightType + " of assigned expression is incompatible with declared type ";
					errStr = errStr + leftType + " of identifier " + leftToken.getText();
					SymbolTable.error(context, rightToken, errStr);
				}
			}
		}

		
		return null;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + operator + '\n';
		
		toRet += left.toStr(newSpaces);
		toRet += right.toStr(newSpaces);
		
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {		
		String leftType = left.getReturnType(attrScope, methScope);
		String rightType = right.getReturnType(attrScope, methScope);
		return checkType(leftType, rightType); 
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}
}
