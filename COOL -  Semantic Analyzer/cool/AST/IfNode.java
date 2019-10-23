package cool.AST;

import java.util.ArrayList;

import org.antlr.v4.runtime.ParserRuleContext;

import cool.structures.DefaultScope;
import cool.structures.Symbol;
import cool.structures.SymbolTable;

public class IfNode implements ASTNode{
	ASTNode ifExpr, thenExpr, elseExpr;
	ParserRuleContext context;
	
	public IfNode(ParserRuleContext context)
	{
		this.context = context;
	}
	
	public void addExpressions(ASTNode ifExpr, ASTNode thenExpr, ASTNode elseExpr)
	{
		this.ifExpr = ifExpr;
		this.thenExpr = thenExpr;
		this.elseExpr = elseExpr;
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = spaces + "if\n";
		
		toRet += ifExpr.toStr(newSpaces);
		toRet += thenExpr.toStr(newSpaces);
		toRet += elseExpr.toStr(newSpaces);
		
		return toRet;
	}

	@Override
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope) {
		String res = ifExpr.getReturnType(attrScope, methScope);
		if(!res.equals("Bool"))
		{
			String errStr = "If condition has type " + res + " instead of Bool";
			SymbolTable.error(context, ifExpr.getContext().start, errStr);
		}
		
		String leftType = thenExpr.getReturnType(attrScope, methScope);
		String rightType = elseExpr.getReturnType(attrScope, methScope);
		ArrayList<String> leftParents = null;
		ArrayList<String> rightParents = null;
		
		if(leftType.equals(rightType))
			return leftType;
		
		if(leftType != null)
		{
			Symbol left = SymbolTable.lookup(leftType);
			if(left != null)
				leftParents = left.getParentList();
		}
		
		if(rightType != null)
		{
			Symbol right = SymbolTable.lookup(rightType);
			if(right != null)
				rightParents = right.getParentList();
		}
		
		if(leftParents != null && rightParents != null)
		{
			ArrayList<String> newLeftParents = new ArrayList<>(leftParents);
			ArrayList<String> newRightParents = new ArrayList<>(rightParents);
			newLeftParents.add(leftType);
			newRightParents.add(rightType);

			newLeftParents.retainAll(rightParents);			//get list intersection
			if(newLeftParents.size() > 0)
				return newLeftParents.get(newLeftParents.size() - 1);
		}
		
		return null;
	}

	@Override
	public ParserRuleContext getContext() {
		return this.context;
	}

}
