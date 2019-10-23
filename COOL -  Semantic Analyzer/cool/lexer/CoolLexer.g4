lexer grammar CoolLexer;

tokens { ERROR } 

@header{
    package cool.lexer;	
}

@members{    
    private void raiseError(String msg) {
        setText(msg);
        setType(ERROR);
    }
   
    private String replaceWhileExists(String str, String toBeRepl, String replWith)
	{
		StringBuilder sb = new StringBuilder(str);
		String newStr = str;
		int index;

		while(newStr.contains(toBeRepl))
		{
			index = sb.indexOf(toBeRepl);
			sb.replace(index, index + 2, replWith);
			newStr = sb.toString();
		}
		return newStr;
	}
    
    private void alterStr(String str)
    {
    	String newStr = replaceWhileExists(str, "\\n", "\n");
		newStr = replaceWhileExists(newStr, "\\t", "\t");
		newStr = replaceWhileExists(newStr, "\\b", "\b");
		newStr = replaceWhileExists(newStr, "\\f", "\f");
		
		StringBuilder sb = new StringBuilder(newStr);
		
		sb.deleteCharAt(0);
		sb.deleteCharAt(sb.toString().length() - 1);
		
		//don't check if the last character is a '\\'
		for(int i = 0; i < sb.toString().length() - 1; i++)
		{
			if(sb.charAt(i) == '\\')
				sb.deleteCharAt(i);
		}
		
		if(sb.toString().length() > 1024)
			raiseError("String constant too long");
		else
			setText(sb.toString());
    }
}

WS : [ \n\f\r\t]+ -> skip;

CLASS : 'class';
INH : 'inherits';

IF: 'if';
THEN : 'then';
ELSE : 'else';
FI : 'fi';

LOOP : 'loop';
POOL : 'pool';

WHILE : 'while';

CASE : 'case';
ESAC : 'esac';

TRUE : 'true';
FALSE : 'false';

LET : 'let';
IN : 'in';

ISVOID: 'isvoid';
NEW : 'new';
OF : 'of';
NOT : 'not';

OPEN_BRACE : '{';
CLOSE_BRACE : '}';
OPEN_BRACKET : '(';
CLOSE_BRACKET : ')';
SEMICOLON : ';';
COLON : ':';
COMMA : ',';

ADD : '+';
SUBST : '-';
MUL : '*';
DIV : '/';
COMPL : '~';

EQ : '=';
LESS : '<';
LESS_OR_EQ : '<=';

ASSIGN : '<-';
DOT : '.';
AT_SIGN : '@';
CASE_OP: '=>';


fragment LETTER : [a-zA-Z];

TYPE_ID : [A-Z] [a-zA-Z0-9_]*;

OBJ_ID : [a-z] [a-zA-Z0-9_]*;

NUM : [0-9]+;

ONE_LINE_COMM: '--' .*? ('\n'|EOF) -> skip;


/* ~[] e folosit ca sa intre pe cazurile de eroare la str */
STR : '"' ( '\\"' | '\\\r\n' | '\\\n' | ~["\n\r\u0000] )*? '"' {alterStr(getText());} ;
UNTERMINATED_STR : '"' ( '\\"' | '\\\r\n' | '\\\n'| ~["\n] )*?  '\n' {raiseError("Unterminated string constant");};
NULL_IN_STR : '"' (  '\u0000'  | '\\"' | '\\\r\n' | '\\\n'  | ~["\n] )*? '"' {raiseError("String contains null character");};
EOF_IN_STR : '"' ( '\\"' |'\\n' | ~["\n] )*? EOF {raiseError("EOF in string constant");};


MULTILINE_COMM : '(*' (MULTILINE_COMM | .)*?  '*)' -> skip;
ERR_MULTI_COMM : '(*' (MULTILINE_COMM | .)*?  ('*)' | EOF {raiseError("EOF in comment");} );
UNMATCHED_OPEN_COMM : '*)' {raiseError("Unmatched *)");};

ERR_CHAR : ~[a-zA-Z0-9_ \n\f\r\t] {raiseError("Invalid character: " + getText());} ;

ERROR : ERR_MULTI_COMM | UNMATCHED_OPEN_COMM | UNTERMINATED_STR | NULL_IN_STR | EOF_IN_STR | ERR_CHAR;


