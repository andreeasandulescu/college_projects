package cool.structures;

import java.io.File;
import java.util.ArrayList;

import org.antlr.v4.runtime.*;

import cool.compiler.Compiler;
import cool.parser.CoolParser;

public class SymbolTable {
    public static Scope globals;
    
    private static boolean semanticErrors;
    
    public static void defineBasicClasses() {
        globals = new DefaultScope("Object", null);
        semanticErrors = false;
        
        ArrayList<String> parList = new ArrayList<String>();
		parList.add("Object");
        
        Symbol sInt = new Symbol("Int", "Int", null);
        Symbol sBool = new Symbol("Bool", "Bool", null);
        Symbol sString = new Symbol("String", "Bool", null);
        Symbol sSelfType = new Symbol("SELF_TYPE", "SELF_TYPE", null);
        Symbol sObject = new Symbol("Object", "Object", null);
        
        sInt.setParentList(parList);
        sBool.setParentList(parList);
        sString.setParentList(parList);
        sSelfType.setParentList(parList);
        
        addSymbolToGlobalScope(sInt);
        addSymbolToGlobalScope(sBool);
        addSymbolToGlobalScope(sString);
        addSymbolToGlobalScope(sSelfType);
        addSymbolToGlobalScope(sObject);
        
    }
    
	//check compatibility of types
    public static boolean checkCompatTypes(String leftType, String rightType, Token leftToken, Token rightToken, ParserRuleContext context)
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
		
		return compatibleType;
    }
   
    public static boolean addSymbolToGlobalScope(Symbol sym)
    {
    	return SymbolTable.globals.add(sym);
    }
    
    public static Symbol lookup(String name)
    {
    	return SymbolTable.globals.lookup(name);
    }
    
    /**
     * Displays a semantic error message.
     * 
     * @param ctx Used to determine the enclosing class context of this error,
     *            which knows the file name in which the class was defined.
     * @param info Used for line and column information.
     * @param str The error message.
     */
    public static void error(ParserRuleContext ctx, Token info, String str) {
    	
        while (! (ctx.getParent() instanceof CoolParser.ProgramContext))
            ctx = ctx.getParent();
        
        String message = "\"" + new File(Compiler.fileNames.get(ctx)).getName()
                + "\", line " + info.getLine()
                + ":" + (info.getCharPositionInLine() + 1)
                + ", Semantic error: " + str;
        
        System.err.println(message);
        
        semanticErrors = true;
    }
    
    public static void error(String str) {
        String message = "Semantic error: " + str;
        
        System.err.println(message);
        
        semanticErrors = true;
    }
    
    public static boolean hasSemanticErrors() {
        return semanticErrors;
    }
}
