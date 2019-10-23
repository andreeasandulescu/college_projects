package cool.structures;

import java.util.*;

public class DefaultScope implements Scope {
    
    private Map<String, Symbol> symbols = new LinkedHashMap<>();
    
    private Scope parent;
    private String name;
    
    public DefaultScope(String name, Scope parent) {
    	this.name = name;
        this.parent = parent;
    }
    
    public String getName()
    {
    	return name;
    }
    
    @Override
    public boolean add(Symbol sym) {
    	
        // Reject duplicates in the same scope.
        if (symbols.containsKey(sym.getName()))
            return false;
        
        symbols.put(sym.getName(), sym);
        
        return true;
    }

    @Override
    public Symbol lookup(String name) {
        var sym = symbols.get(name);
        
        if (sym != null)
            return sym;
        
        if (parent != null)
            return parent.lookup(name);
        
        return null;
    }

    public void setParent(Scope parent)
    {
    	this.parent = parent;
    }
    
    @Override
    public Scope getParent() {
        return parent;
    }
    
    @Override
    public String toString() {
        return symbols.values().toString();
    }

}
