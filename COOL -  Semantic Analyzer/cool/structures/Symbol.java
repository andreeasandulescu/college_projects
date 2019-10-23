package cool.structures;

import java.util.ArrayList;

import cool.AST.ASTNode;

public class Symbol {
    protected String name, type;
    public ASTNode node;
    public ArrayList<String> parentList;
    
    public Symbol(String name, String type, ASTNode node) {
        this.name = name;
        this.type = type;
        this.node = node;
        this.parentList = null;
    }
    
    public void setParentList(ArrayList<String> parentList)
    {
    	this.parentList = parentList;
    }
    
    public ArrayList<String> getParentList()
    {
    	return parentList;
    }
    
    public String getType()
    {
    	return type;
    }
    
    public String getName() {
        return name;
    }
    
    public ASTNode getASTNode()
    {
    	return node;
    }
    
    @Override
    public String toString() {
        return getName();
    }

}
