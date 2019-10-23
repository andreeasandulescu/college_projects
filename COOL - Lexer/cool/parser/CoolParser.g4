parser grammar CoolParser;

options {
    tokenVocab = CoolLexer;
}

@header{
    package cool.parser;
}

program : (classes+=class_def SEMICOLON)+ ; 

class_def : CLASS name=TYPE_ID (INH parName=TYPE_ID)? OPEN_BRACE (features+=feature SEMICOLON)* CLOSE_BRACE;

feature 
	: name=OBJ_ID OPEN_BRACKET (params+=formal (COMMA params+=formal)*)? CLOSE_BRACKET COLON 
	  type=TYPE_ID OPEN_BRACE body=expr CLOSE_BRACE															#meth
	| name=OBJ_ID COLON type=TYPE_ID (ASSIGN val=expr)?														#attr
	;

formal : name=OBJ_ID COLON type=TYPE_ID;
attrib: name=OBJ_ID COLON type=TYPE_ID (ASSIGN val=expr)?;
case_attrib : name=OBJ_ID COLON type=TYPE_ID CASE_OP val=expr SEMICOLON;

expr 
	: exp=expr (AT_SIGN type=TYPE_ID)? DOT 
	  name=OBJ_ID OPEN_BRACKET (values+=expr (COMMA values+=expr)* )? CLOSE_BRACKET							#memb_fun_call
	| name=OBJ_ID OPEN_BRACKET (values+=expr (COMMA values+=expr)* )? CLOSE_BRACKET							#fun_call
	| IF if_e=expr THEN then_e=expr ELSE else_e=expr FI 													#if_instr
	| WHILE while_e=expr LOOP loop_e=expr POOL																#while_instr
	| OPEN_BRACE (values+=expr SEMICOLON)+ CLOSE_BRACE														#block
	| LET params+=attrib (COMMA params+=attrib )* IN exp=expr												#let_instr
	| CASE exp=expr OF (values+=case_attrib)+ ESAC							#case_instr
	| NEW name=TYPE_ID																						#new
	| op=COMPL exp=expr																						#compl
	| op=ISVOID exp=expr																					#isvoid
	| OPEN_BRACKET exp=expr CLOSE_BRACKET																	#bracket_expr
	| left=expr op=MUL right=expr																			#mul
	| left=expr op=DIV right=expr																			#div
	| left=expr op=ADD right=expr																			#add
	| left=expr op=SUBST right=expr																			#subst
	| left=expr op=LESS_OR_EQ right=expr																	#less_or_eq
	| left=expr op=LESS right=expr																			#less
	| left=expr op=EQ right=expr																			#eq
	| op=NOT exp=expr 																						#not
	| left=OBJ_ID op=ASSIGN right=expr																		#assign
	| OBJ_ID																								#id
	| NUM																									#num
	| STR																									#str
	| TRUE																									#true
	| FALSE																									#false
	; 

error 
	: UNTERMINATED_STR																					  	#unterm_str
	| NULL_IN_STR																							#null_in_str
	| EOF_IN_STR																							#eof_in_str
	| UNMATCHED_OPEN_COMM																					#unmatch_comm
	| ERR_MULTI_COMM																						#err_multi_comm
	| ERR_CHAR																								#err_char
	;
	
	