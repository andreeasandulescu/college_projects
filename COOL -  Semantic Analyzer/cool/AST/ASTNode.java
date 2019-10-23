package cool.AST;

import org.antlr.v4.runtime.ParserRuleContext;
import cool.structures.DefaultScope;

public interface ASTNode {
	public String toStr(String spaces);	//spaces = identarea pentru nodul curent
	public String getReturnType(DefaultScope attrScope, DefaultScope methScope);
	public ParserRuleContext getContext();
}

