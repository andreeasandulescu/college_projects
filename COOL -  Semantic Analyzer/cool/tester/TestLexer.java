package cool.tester;

import java.io.IOException;
import org.antlr.v4.runtime.*;

import cool.lexer.CoolLexer;
import cool.parser.CoolParser;
import cool.parser.MyParseTreeVisitor;


public class TestLexer {
	
    public static void main(String[] args) throws IOException {
        var input = CharStreams.fromFileName("./tests/tema2/01-define-class.cl");
        
        var lexer = new CoolLexer(input);
        var tokenStream = new CommonTokenStream(lexer);
       
        tokenStream.fill();
        //List<Token> tokens = tokenStream.getTokens();
        
        var parser = new CoolParser(tokenStream);
        var antlr_tree = parser.program();
        var myVisitor = new MyParseTreeVisitor();
        var root = antlr_tree.accept(myVisitor);
        String s = root.toStr("");
        System.out.print(s);
    }
    
}
