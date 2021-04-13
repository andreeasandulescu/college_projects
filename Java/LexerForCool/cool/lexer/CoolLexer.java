// Generated from CoolLexer.g4 by ANTLR 4.7.1

    package cool.lexer;	

import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class CoolLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.7.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		ERROR=1, WS=2, CLASS=3, INH=4, IF=5, THEN=6, ELSE=7, FI=8, LOOP=9, POOL=10, 
		WHILE=11, CASE=12, ESAC=13, TRUE=14, FALSE=15, LET=16, IN=17, ISVOID=18, 
		NEW=19, OF=20, NOT=21, OPEN_BRACE=22, CLOSE_BRACE=23, OPEN_BRACKET=24, 
		CLOSE_BRACKET=25, SEMICOLON=26, COLON=27, COMMA=28, ADD=29, SUBST=30, 
		MUL=31, DIV=32, COMPL=33, EQ=34, LESS=35, LESS_OR_EQ=36, ASSIGN=37, DOT=38, 
		AT_SIGN=39, CASE_OP=40, TYPE_ID=41, OBJ_ID=42, NUM=43, ONE_LINE_COMM=44, 
		STR=45, UNTERMINATED_STR=46, NULL_IN_STR=47, EOF_IN_STR=48, MULTILINE_COMM=49, 
		ERR_MULTI_COMM=50, UNMATCHED_OPEN_COMM=51, ERR_CHAR=52;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	public static final String[] ruleNames = {
		"WS", "CLASS", "INH", "IF", "THEN", "ELSE", "FI", "LOOP", "POOL", "WHILE", 
		"CASE", "ESAC", "TRUE", "FALSE", "LET", "IN", "ISVOID", "NEW", "OF", "NOT", 
		"OPEN_BRACE", "CLOSE_BRACE", "OPEN_BRACKET", "CLOSE_BRACKET", "SEMICOLON", 
		"COLON", "COMMA", "ADD", "SUBST", "MUL", "DIV", "COMPL", "EQ", "LESS", 
		"LESS_OR_EQ", "ASSIGN", "DOT", "AT_SIGN", "CASE_OP", "TYPE_ID", "OBJ_ID", 
		"NUM", "ONE_LINE_COMM", "STR", "UNTERMINATED_STR", "NULL_IN_STR", "EOF_IN_STR", 
		"MULTILINE_COMM", "ERR_MULTI_COMM", "UNMATCHED_OPEN_COMM", "ERR_CHAR", 
		"ERROR"
	};

	private static final String[] _LITERAL_NAMES = {
		null, null, null, "'class'", "'inherits'", "'if'", "'then'", "'else'", 
		"'fi'", "'loop'", "'pool'", "'while'", "'case'", "'esac'", "'true'", "'false'", 
		"'let'", "'in'", "'isvoid'", "'new'", "'of'", "'not'", "'{'", "'}'", "'('", 
		"')'", "';'", "':'", "','", "'+'", "'-'", "'*'", "'/'", "'~'", "'='", 
		"'<'", "'<='", "'<-'", "'.'", "'@'", "'=>'", null, null, null, null, null, 
		null, null, null, null, null, "'*)'"
	};
	private static final String[] _SYMBOLIC_NAMES = {
		null, "ERROR", "WS", "CLASS", "INH", "IF", "THEN", "ELSE", "FI", "LOOP", 
		"POOL", "WHILE", "CASE", "ESAC", "TRUE", "FALSE", "LET", "IN", "ISVOID", 
		"NEW", "OF", "NOT", "OPEN_BRACE", "CLOSE_BRACE", "OPEN_BRACKET", "CLOSE_BRACKET", 
		"SEMICOLON", "COLON", "COMMA", "ADD", "SUBST", "MUL", "DIV", "COMPL", 
		"EQ", "LESS", "LESS_OR_EQ", "ASSIGN", "DOT", "AT_SIGN", "CASE_OP", "TYPE_ID", 
		"OBJ_ID", "NUM", "ONE_LINE_COMM", "STR", "UNTERMINATED_STR", "NULL_IN_STR", 
		"EOF_IN_STR", "MULTILINE_COMM", "ERR_MULTI_COMM", "UNMATCHED_OPEN_COMM", 
		"ERR_CHAR"
	};
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	    
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


	public CoolLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "CoolLexer.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	@Override
	public void action(RuleContext _localctx, int ruleIndex, int actionIndex) {
		switch (ruleIndex) {
		case 43:
			STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 44:
			UNTERMINATED_STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 45:
			NULL_IN_STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 46:
			EOF_IN_STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 48:
			ERR_MULTI_COMM_action((RuleContext)_localctx, actionIndex);
			break;
		case 49:
			UNMATCHED_OPEN_COMM_action((RuleContext)_localctx, actionIndex);
			break;
		case 50:
			ERR_CHAR_action((RuleContext)_localctx, actionIndex);
			break;
		}
	}
	private void STR_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 0:
			alterStr(getText());
			break;
		}
	}
	private void UNTERMINATED_STR_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 1:
			raiseError("Unterminated string constant");
			break;
		}
	}
	private void NULL_IN_STR_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 2:
			raiseError("String contains null character");
			break;
		}
	}
	private void EOF_IN_STR_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 3:
			raiseError("EOF in string constant");
			break;
		}
	}
	private void ERR_MULTI_COMM_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 4:
			raiseError("EOF in comment");
			break;
		}
	}
	private void UNMATCHED_OPEN_COMM_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 5:
			raiseError("Unmatched *)");
			break;
		}
	}
	private void ERR_CHAR_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 6:
			raiseError("Invalid character: " + getText());
			break;
		}
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\66\u018a\b\1\4\2"+
		"\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4"+
		"\13\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22"+
		"\t\22\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31"+
		"\t\31\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t"+
		" \4!\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t"+
		"+\4,\t,\4-\t-\4.\t.\4/\t/\4\60\t\60\4\61\t\61\4\62\t\62\4\63\t\63\4\64"+
		"\t\64\4\65\t\65\3\2\6\2m\n\2\r\2\16\2n\3\2\3\2\3\3\3\3\3\3\3\3\3\3\3\3"+
		"\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3\6\3"+
		"\7\3\7\3\7\3\7\3\7\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3\n\3\n\3\n\3\n\3\n"+
		"\3\13\3\13\3\13\3\13\3\13\3\13\3\f\3\f\3\f\3\f\3\f\3\r\3\r\3\r\3\r\3\r"+
		"\3\16\3\16\3\16\3\16\3\16\3\17\3\17\3\17\3\17\3\17\3\17\3\20\3\20\3\20"+
		"\3\20\3\21\3\21\3\21\3\22\3\22\3\22\3\22\3\22\3\22\3\22\3\23\3\23\3\23"+
		"\3\23\3\24\3\24\3\24\3\25\3\25\3\25\3\25\3\26\3\26\3\27\3\27\3\30\3\30"+
		"\3\31\3\31\3\32\3\32\3\33\3\33\3\34\3\34\3\35\3\35\3\36\3\36\3\37\3\37"+
		"\3 \3 \3!\3!\3\"\3\"\3#\3#\3$\3$\3$\3%\3%\3%\3&\3&\3\'\3\'\3(\3(\3(\3"+
		")\3)\7)\u00fb\n)\f)\16)\u00fe\13)\3*\3*\7*\u0102\n*\f*\16*\u0105\13*\3"+
		"+\6+\u0108\n+\r+\16+\u0109\3,\3,\3,\3,\7,\u0110\n,\f,\16,\u0113\13,\3"+
		",\5,\u0116\n,\3,\3,\3-\3-\3-\3-\3-\3-\3-\3-\3-\7-\u0123\n-\f-\16-\u0126"+
		"\13-\3-\3-\3-\3.\3.\3.\3.\3.\3.\3.\3.\3.\7.\u0134\n.\f.\16.\u0137\13."+
		"\3.\3.\3.\3/\3/\3/\3/\3/\3/\3/\3/\3/\3/\7/\u0146\n/\f/\16/\u0149\13/\3"+
		"/\3/\3/\3\60\3\60\3\60\3\60\3\60\3\60\7\60\u0154\n\60\f\60\16\60\u0157"+
		"\13\60\3\60\3\60\3\60\3\61\3\61\3\61\3\61\3\61\7\61\u0161\n\61\f\61\16"+
		"\61\u0164\13\61\3\61\3\61\3\61\3\61\3\61\3\62\3\62\3\62\3\62\3\62\7\62"+
		"\u0170\n\62\f\62\16\62\u0173\13\62\3\62\3\62\3\62\3\62\5\62\u0179\n\62"+
		"\3\63\3\63\3\63\3\63\3\63\3\64\3\64\3\64\3\65\3\65\3\65\3\65\3\65\3\65"+
		"\5\65\u0189\n\65\t\u0111\u0124\u0135\u0147\u0155\u0162\u0171\2\66\3\4"+
		"\5\5\7\6\t\7\13\b\r\t\17\n\21\13\23\f\25\r\27\16\31\17\33\20\35\21\37"+
		"\22!\23#\24%\25\'\26)\27+\30-\31/\32\61\33\63\34\65\35\67\369\37; =!?"+
		"\"A#C$E%G&I\'K(M)O*Q+S,U-W.Y/[\60]\61_\62a\63c\64e\65g\66i\3\3\2\13\5"+
		"\2\13\f\16\17\"\"\3\2C\\\6\2\62;C\\aac|\3\2c|\3\2\62;\3\3\f\f\6\2\2\2"+
		"\f\f\17\17$$\4\2\f\f$$\t\2\13\f\16\17\"\"\62;C\\aac|\2\u01a8\2\3\3\2\2"+
		"\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3"+
		"\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2"+
		"\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2"+
		"\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2"+
		"\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3"+
		"\2\2\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2\2\2\2E\3\2\2\2\2G\3\2\2\2\2I\3\2\2"+
		"\2\2K\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2\2Q\3\2\2\2\2S\3\2\2\2\2U\3\2\2\2\2"+
		"W\3\2\2\2\2Y\3\2\2\2\2[\3\2\2\2\2]\3\2\2\2\2_\3\2\2\2\2a\3\2\2\2\2c\3"+
		"\2\2\2\2e\3\2\2\2\2g\3\2\2\2\2i\3\2\2\2\3l\3\2\2\2\5r\3\2\2\2\7x\3\2\2"+
		"\2\t\u0081\3\2\2\2\13\u0084\3\2\2\2\r\u0089\3\2\2\2\17\u008e\3\2\2\2\21"+
		"\u0091\3\2\2\2\23\u0096\3\2\2\2\25\u009b\3\2\2\2\27\u00a1\3\2\2\2\31\u00a6"+
		"\3\2\2\2\33\u00ab\3\2\2\2\35\u00b0\3\2\2\2\37\u00b6\3\2\2\2!\u00ba\3\2"+
		"\2\2#\u00bd\3\2\2\2%\u00c4\3\2\2\2\'\u00c8\3\2\2\2)\u00cb\3\2\2\2+\u00cf"+
		"\3\2\2\2-\u00d1\3\2\2\2/\u00d3\3\2\2\2\61\u00d5\3\2\2\2\63\u00d7\3\2\2"+
		"\2\65\u00d9\3\2\2\2\67\u00db\3\2\2\29\u00dd\3\2\2\2;\u00df\3\2\2\2=\u00e1"+
		"\3\2\2\2?\u00e3\3\2\2\2A\u00e5\3\2\2\2C\u00e7\3\2\2\2E\u00e9\3\2\2\2G"+
		"\u00eb\3\2\2\2I\u00ee\3\2\2\2K\u00f1\3\2\2\2M\u00f3\3\2\2\2O\u00f5\3\2"+
		"\2\2Q\u00f8\3\2\2\2S\u00ff\3\2\2\2U\u0107\3\2\2\2W\u010b\3\2\2\2Y\u0119"+
		"\3\2\2\2[\u012a\3\2\2\2]\u013b\3\2\2\2_\u014d\3\2\2\2a\u015b\3\2\2\2c"+
		"\u016a\3\2\2\2e\u017a\3\2\2\2g\u017f\3\2\2\2i\u0188\3\2\2\2km\t\2\2\2"+
		"lk\3\2\2\2mn\3\2\2\2nl\3\2\2\2no\3\2\2\2op\3\2\2\2pq\b\2\2\2q\4\3\2\2"+
		"\2rs\7e\2\2st\7n\2\2tu\7c\2\2uv\7u\2\2vw\7u\2\2w\6\3\2\2\2xy\7k\2\2yz"+
		"\7p\2\2z{\7j\2\2{|\7g\2\2|}\7t\2\2}~\7k\2\2~\177\7v\2\2\177\u0080\7u\2"+
		"\2\u0080\b\3\2\2\2\u0081\u0082\7k\2\2\u0082\u0083\7h\2\2\u0083\n\3\2\2"+
		"\2\u0084\u0085\7v\2\2\u0085\u0086\7j\2\2\u0086\u0087\7g\2\2\u0087\u0088"+
		"\7p\2\2\u0088\f\3\2\2\2\u0089\u008a\7g\2\2\u008a\u008b\7n\2\2\u008b\u008c"+
		"\7u\2\2\u008c\u008d\7g\2\2\u008d\16\3\2\2\2\u008e\u008f\7h\2\2\u008f\u0090"+
		"\7k\2\2\u0090\20\3\2\2\2\u0091\u0092\7n\2\2\u0092\u0093\7q\2\2\u0093\u0094"+
		"\7q\2\2\u0094\u0095\7r\2\2\u0095\22\3\2\2\2\u0096\u0097\7r\2\2\u0097\u0098"+
		"\7q\2\2\u0098\u0099\7q\2\2\u0099\u009a\7n\2\2\u009a\24\3\2\2\2\u009b\u009c"+
		"\7y\2\2\u009c\u009d\7j\2\2\u009d\u009e\7k\2\2\u009e\u009f\7n\2\2\u009f"+
		"\u00a0\7g\2\2\u00a0\26\3\2\2\2\u00a1\u00a2\7e\2\2\u00a2\u00a3\7c\2\2\u00a3"+
		"\u00a4\7u\2\2\u00a4\u00a5\7g\2\2\u00a5\30\3\2\2\2\u00a6\u00a7\7g\2\2\u00a7"+
		"\u00a8\7u\2\2\u00a8\u00a9\7c\2\2\u00a9\u00aa\7e\2\2\u00aa\32\3\2\2\2\u00ab"+
		"\u00ac\7v\2\2\u00ac\u00ad\7t\2\2\u00ad\u00ae\7w\2\2\u00ae\u00af\7g\2\2"+
		"\u00af\34\3\2\2\2\u00b0\u00b1\7h\2\2\u00b1\u00b2\7c\2\2\u00b2\u00b3\7"+
		"n\2\2\u00b3\u00b4\7u\2\2\u00b4\u00b5\7g\2\2\u00b5\36\3\2\2\2\u00b6\u00b7"+
		"\7n\2\2\u00b7\u00b8\7g\2\2\u00b8\u00b9\7v\2\2\u00b9 \3\2\2\2\u00ba\u00bb"+
		"\7k\2\2\u00bb\u00bc\7p\2\2\u00bc\"\3\2\2\2\u00bd\u00be\7k\2\2\u00be\u00bf"+
		"\7u\2\2\u00bf\u00c0\7x\2\2\u00c0\u00c1\7q\2\2\u00c1\u00c2\7k\2\2\u00c2"+
		"\u00c3\7f\2\2\u00c3$\3\2\2\2\u00c4\u00c5\7p\2\2\u00c5\u00c6\7g\2\2\u00c6"+
		"\u00c7\7y\2\2\u00c7&\3\2\2\2\u00c8\u00c9\7q\2\2\u00c9\u00ca\7h\2\2\u00ca"+
		"(\3\2\2\2\u00cb\u00cc\7p\2\2\u00cc\u00cd\7q\2\2\u00cd\u00ce\7v\2\2\u00ce"+
		"*\3\2\2\2\u00cf\u00d0\7}\2\2\u00d0,\3\2\2\2\u00d1\u00d2\7\177\2\2\u00d2"+
		".\3\2\2\2\u00d3\u00d4\7*\2\2\u00d4\60\3\2\2\2\u00d5\u00d6\7+\2\2\u00d6"+
		"\62\3\2\2\2\u00d7\u00d8\7=\2\2\u00d8\64\3\2\2\2\u00d9\u00da\7<\2\2\u00da"+
		"\66\3\2\2\2\u00db\u00dc\7.\2\2\u00dc8\3\2\2\2\u00dd\u00de\7-\2\2\u00de"+
		":\3\2\2\2\u00df\u00e0\7/\2\2\u00e0<\3\2\2\2\u00e1\u00e2\7,\2\2\u00e2>"+
		"\3\2\2\2\u00e3\u00e4\7\61\2\2\u00e4@\3\2\2\2\u00e5\u00e6\7\u0080\2\2\u00e6"+
		"B\3\2\2\2\u00e7\u00e8\7?\2\2\u00e8D\3\2\2\2\u00e9\u00ea\7>\2\2\u00eaF"+
		"\3\2\2\2\u00eb\u00ec\7>\2\2\u00ec\u00ed\7?\2\2\u00edH\3\2\2\2\u00ee\u00ef"+
		"\7>\2\2\u00ef\u00f0\7/\2\2\u00f0J\3\2\2\2\u00f1\u00f2\7\60\2\2\u00f2L"+
		"\3\2\2\2\u00f3\u00f4\7B\2\2\u00f4N\3\2\2\2\u00f5\u00f6\7?\2\2\u00f6\u00f7"+
		"\7@\2\2\u00f7P\3\2\2\2\u00f8\u00fc\t\3\2\2\u00f9\u00fb\t\4\2\2\u00fa\u00f9"+
		"\3\2\2\2\u00fb\u00fe\3\2\2\2\u00fc\u00fa\3\2\2\2\u00fc\u00fd\3\2\2\2\u00fd"+
		"R\3\2\2\2\u00fe\u00fc\3\2\2\2\u00ff\u0103\t\5\2\2\u0100\u0102\t\4\2\2"+
		"\u0101\u0100\3\2\2\2\u0102\u0105\3\2\2\2\u0103\u0101\3\2\2\2\u0103\u0104"+
		"\3\2\2\2\u0104T\3\2\2\2\u0105\u0103\3\2\2\2\u0106\u0108\t\6\2\2\u0107"+
		"\u0106\3\2\2\2\u0108\u0109\3\2\2\2\u0109\u0107\3\2\2\2\u0109\u010a\3\2"+
		"\2\2\u010aV\3\2\2\2\u010b\u010c\7/\2\2\u010c\u010d\7/\2\2\u010d\u0111"+
		"\3\2\2\2\u010e\u0110\13\2\2\2\u010f\u010e\3\2\2\2\u0110\u0113\3\2\2\2"+
		"\u0111\u0112\3\2\2\2\u0111\u010f\3\2\2\2\u0112\u0115\3\2\2\2\u0113\u0111"+
		"\3\2\2\2\u0114\u0116\t\7\2\2\u0115\u0114\3\2\2\2\u0116\u0117\3\2\2\2\u0117"+
		"\u0118\b,\2\2\u0118X\3\2\2\2\u0119\u0124\7$\2\2\u011a\u011b\7^\2\2\u011b"+
		"\u0123\7$\2\2\u011c\u011d\7^\2\2\u011d\u011e\7\17\2\2\u011e\u0123\7\f"+
		"\2\2\u011f\u0120\7^\2\2\u0120\u0123\7\f\2\2\u0121\u0123\n\b\2\2\u0122"+
		"\u011a\3\2\2\2\u0122\u011c\3\2\2\2\u0122\u011f\3\2\2\2\u0122\u0121\3\2"+
		"\2\2\u0123\u0126\3\2\2\2\u0124\u0125\3\2\2\2\u0124\u0122\3\2\2\2\u0125"+
		"\u0127\3\2\2\2\u0126\u0124\3\2\2\2\u0127\u0128\7$\2\2\u0128\u0129\b-\3"+
		"\2\u0129Z\3\2\2\2\u012a\u0135\7$\2\2\u012b\u012c\7^\2\2\u012c\u0134\7"+
		"$\2\2\u012d\u012e\7^\2\2\u012e\u012f\7\17\2\2\u012f\u0134\7\f\2\2\u0130"+
		"\u0131\7^\2\2\u0131\u0134\7\f\2\2\u0132\u0134\n\t\2\2\u0133\u012b\3\2"+
		"\2\2\u0133\u012d\3\2\2\2\u0133\u0130\3\2\2\2\u0133\u0132\3\2\2\2\u0134"+
		"\u0137\3\2\2\2\u0135\u0136\3\2\2\2\u0135\u0133\3\2\2\2\u0136\u0138\3\2"+
		"\2\2\u0137\u0135\3\2\2\2\u0138\u0139\7\f\2\2\u0139\u013a\b.\4\2\u013a"+
		"\\\3\2\2\2\u013b\u0147\7$\2\2\u013c\u0146\7\2\2\2\u013d\u013e\7^\2\2\u013e"+
		"\u0146\7$\2\2\u013f\u0140\7^\2\2\u0140\u0141\7\17\2\2\u0141\u0146\7\f"+
		"\2\2\u0142\u0143\7^\2\2\u0143\u0146\7\f\2\2\u0144\u0146\n\t\2\2\u0145"+
		"\u013c\3\2\2\2\u0145\u013d\3\2\2\2\u0145\u013f\3\2\2\2\u0145\u0142\3\2"+
		"\2\2\u0145\u0144\3\2\2\2\u0146\u0149\3\2\2\2\u0147\u0148\3\2\2\2\u0147"+
		"\u0145\3\2\2\2\u0148\u014a\3\2\2\2\u0149\u0147\3\2\2\2\u014a\u014b\7$"+
		"\2\2\u014b\u014c\b/\5\2\u014c^\3\2\2\2\u014d\u0155\7$\2\2\u014e\u014f"+
		"\7^\2\2\u014f\u0154\7$\2\2\u0150\u0151\7^\2\2\u0151\u0154\7p\2\2\u0152"+
		"\u0154\n\t\2\2\u0153\u014e\3\2\2\2\u0153\u0150\3\2\2\2\u0153\u0152\3\2"+
		"\2\2\u0154\u0157\3\2\2\2\u0155\u0156\3\2\2\2\u0155\u0153\3\2\2\2\u0156"+
		"\u0158\3\2\2\2\u0157\u0155\3\2\2\2\u0158\u0159\7\2\2\3\u0159\u015a\b\60"+
		"\6\2\u015a`\3\2\2\2\u015b\u015c\7*\2\2\u015c\u015d\7,\2\2\u015d\u0162"+
		"\3\2\2\2\u015e\u0161\5a\61\2\u015f\u0161\13\2\2\2\u0160\u015e\3\2\2\2"+
		"\u0160\u015f\3\2\2\2\u0161\u0164\3\2\2\2\u0162\u0163\3\2\2\2\u0162\u0160"+
		"\3\2\2\2\u0163\u0165\3\2\2\2\u0164\u0162\3\2\2\2\u0165\u0166\7,\2\2\u0166"+
		"\u0167\7+\2\2\u0167\u0168\3\2\2\2\u0168\u0169\b\61\2\2\u0169b\3\2\2\2"+
		"\u016a\u016b\7*\2\2\u016b\u016c\7,\2\2\u016c\u0171\3\2\2\2\u016d\u0170"+
		"\5a\61\2\u016e\u0170\13\2\2\2\u016f\u016d\3\2\2\2\u016f\u016e\3\2\2\2"+
		"\u0170\u0173\3\2\2\2\u0171\u0172\3\2\2\2\u0171\u016f\3\2\2\2\u0172\u0178"+
		"\3\2\2\2\u0173\u0171\3\2\2\2\u0174\u0175\7,\2\2\u0175\u0179\7+\2\2\u0176"+
		"\u0177\7\2\2\3\u0177\u0179\b\62\7\2\u0178\u0174\3\2\2\2\u0178\u0176\3"+
		"\2\2\2\u0179d\3\2\2\2\u017a\u017b\7,\2\2\u017b\u017c\7+\2\2\u017c\u017d"+
		"\3\2\2\2\u017d\u017e\b\63\b\2\u017ef\3\2\2\2\u017f\u0180\n\n\2\2\u0180"+
		"\u0181\b\64\t\2\u0181h\3\2\2\2\u0182\u0189\5c\62\2\u0183\u0189\5e\63\2"+
		"\u0184\u0189\5[.\2\u0185\u0189\5]/\2\u0186\u0189\5_\60\2\u0187\u0189\5"+
		"g\64\2\u0188\u0182\3\2\2\2\u0188\u0183\3\2\2\2\u0188\u0184\3\2\2\2\u0188"+
		"\u0185\3\2\2\2\u0188\u0186\3\2\2\2\u0188\u0187\3\2\2\2\u0189j\3\2\2\2"+
		"\27\2n\u00fc\u0103\u0109\u0111\u0115\u0122\u0124\u0133\u0135\u0145\u0147"+
		"\u0153\u0155\u0160\u0162\u016f\u0171\u0178\u0188\n\b\2\2\3-\2\3.\3\3/"+
		"\4\3\60\5\3\62\6\3\63\7\3\64\b";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}