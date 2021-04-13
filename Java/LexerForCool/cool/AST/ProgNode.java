package cool.AST;

import java.util.ArrayList;
import java.util.List;

public class ProgNode implements ASTNode{
	
	List<ClassNode> classes = new ArrayList<ClassNode>();

	public void addChild(ClassNode c) {
		classes.add(c);
	}
	
	@Override
	public String toStr(String spaces) {
		String newSpaces = spaces + "  ";
		String toRet = "program\n";
		
		for(ClassNode node : classes)
			toRet += node.toStr(newSpaces);
		
		return toRet;
	}
	
}
