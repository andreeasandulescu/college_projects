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
		"LESS_OR_EQ", "ASSIGN", "DOT", "AT_SIGN", "CASE_OP", "LETTER", "TYPE_ID", 
		"OBJ_ID", "NUM", "ONE_LINE_COMM", "STR", "UNTERMINATED_STR", "NULL_IN_STR", 
		"EOF_IN_STR", "MULTILINE_COMM", "ERR_MULTI_COMM", "UNMATCHED_OPEN_COMM", 
		"ERR_CHAR", "ERROR"
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
		case 44:
			STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 45:
			UNTERMINATED_STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 46:
			NULL_IN_STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 47:
			EOF_IN_STR_action((RuleContext)_localctx, actionIndex);
			break;
		case 49:
			ERR_MULTI_COMM_action((RuleContext)_localctx, actionIndex);
			break;
		case 50:
			UNMATCHED_OPEN_COMM_action((RuleContext)_localctx, actionIndex);
			break;
		case 51:
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
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\66\u018e\b\1\4\2"+
		"\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4"+
		"\13\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22"+
		"\t\22\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31"+
		"\t\31\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t"+
		" \4!\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t"+
		"+\4,\t,\4-\t-\4.\t.\4/\t/\4\60\t\60\4\61\t\61\4\62\t\62\4\63\t\63\4\64"+
		"\t\64\4\65\t\65\4\66\t\66\3\2\6\2o\n\2\r\2\16\2p\3\2\3\2\3\3\3\3\3\3\3"+
		"\3\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\6\3\6\3\6"+
		"\3\6\3\6\3\7\3\7\3\7\3\7\3\7\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3\n\3\n\3"+
		"\n\3\n\3\n\3\13\3\13\3\13\3\13\3\13\3\13\3\f\3\f\3\f\3\f\3\f\3\r\3\r\3"+
		"\r\3\r\3\r\3\16\3\16\3\16\3\16\3\16\3\17\3\17\3\17\3\17\3\17\3\17\3\20"+
		"\3\20\3\20\3\20\3\21\3\21\3\21\3\22\3\22\3\22\3\22\3\22\3\22\3\22\3\23"+
		"\3\23\3\23\3\23\3\24\3\24\3\24\3\25\3\25\3\25\3\25\3\26\3\26\3\27\3\27"+
		"\3\30\3\30\3\31\3\31\3\32\3\32\3\33\3\33\3\34\3\34\3\35\3\35\3\36\3\36"+
		"\3\37\3\37\3 \3 \3!\3!\3\"\3\"\3#\3#\3$\3$\3$\3%\3%\3%\3&\3&\3\'\3\'\3"+
		"(\3(\3(\3)\3)\3*\3*\7*\u00ff\n*\f*\16*\u0102\13*\3+\3+\7+\u0106\n+\f+"+
		"\16+\u0109\13+\3,\6,\u010c\n,\r,\16,\u010d\3-\3-\3-\3-\7-\u0114\n-\f-"+
		"\16-\u0117\13-\3-\5-\u011a\n-\3-\3-\3.\3.\3.\3.\3.\3.\3.\3.\3.\7.\u0127"+
		"\n.\f.\16.\u012a\13.\3.\3.\3.\3/\3/\3/\3/\3/\3/\3/\3/\3/\7/\u0138\n/\f"+
		"/\16/\u013b\13/\3/\3/\3/\3\60\3\60\3\60\3\60\3\60\3\60\3\60\3\60\3\60"+
		"\3\60\7\60\u014a\n\60\f\60\16\60\u014d\13\60\3\60\3\60\3\60\3\61\3\61"+
		"\3\61\3\61\3\61\3\61\7\61\u0158\n\61\f\61\16\61\u015b\13\61\3\61\3\61"+
		"\3\61\3\62\3\62\3\62\3\62\3\62\7\62\u0165\n\62\f\62\16\62\u0168\13\62"+
		"\3\62\3\62\3\62\3\62\3\62\3\63\3\63\3\63\3\63\3\63\7\63\u0174\n\63\f\63"+
		"\16\63\u0177\13\63\3\63\3\63\3\63\3\63\5\63\u017d\n\63\3\64\3\64\3\64"+
		"\3\64\3\64\3\65\3\65\3\65\3\66\3\66\3\66\3\66\3\66\3\66\5\66\u018d\n\66"+
		"\t\u0115\u0128\u0139\u014b\u0159\u0166\u0175\2\67\3\4\5\5\7\6\t\7\13\b"+
		"\r\t\17\n\21\13\23\f\25\r\27\16\31\17\33\20\35\21\37\22!\23#\24%\25\'"+
		"\26)\27+\30-\31/\32\61\33\63\34\65\35\67\369\37; =!?\"A#C$E%G&I\'K(M)"+
		"O*Q\2S+U,W-Y.[/]\60_\61a\62c\63e\64g\65i\66k\3\3\2\f\5\2\13\f\16\17\""+
		"\"\4\2C\\c|\3\2C\\\6\2\62;C\\aac|\3\2c|\3\2\62;\3\3\f\f\6\2\2\2\f\f\17"+
		"\17$$\4\2\f\f$$\t\2\13\f\16\17\"\"\62;C\\aac|\2\u01ab\2\3\3\2\2\2\2\5"+
		"\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2"+
		"\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33"+
		"\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2\2\2"+
		"\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2\2"+
		"\63\3\2\2\2\2\65\3\2\2\2\2\67\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3\2\2\2"+
		"\2?\3\2\2\2\2A\3\2\2\2\2C\3\2\2\2\2E\3\2\2\2\2G\3\2\2\2\2I\3\2\2\2\2K"+
		"\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2\2S\3\2\2\2\2U\3\2\2\2\2W\3\2\2\2\2Y\3\2"+
		"\2\2\2[\3\2\2\2\2]\3\2\2\2\2_\3\2\2\2\2a\3\2\2\2\2c\3\2\2\2\2e\3\2\2\2"+
		"\2g\3\2\2\2\2i\3\2\2\2\2k\3\2\2\2\3n\3\2\2\2\5t\3\2\2\2\7z\3\2\2\2\t\u0083"+
		"\3\2\2\2\13\u0086\3\2\2\2\r\u008b\3\2\2\2\17\u0090\3\2\2\2\21\u0093\3"+
		"\2\2\2\23\u0098\3\2\2\2\25\u009d\3\2\2\2\27\u00a3\3\2\2\2\31\u00a8\3\2"+
		"\2\2\33\u00ad\3\2\2\2\35\u00b2\3\2\2\2\37\u00b8\3\2\2\2!\u00bc\3\2\2\2"+
		"#\u00bf\3\2\2\2%\u00c6\3\2\2\2\'\u00ca\3\2\2\2)\u00cd\3\2\2\2+\u00d1\3"+
		"\2\2\2-\u00d3\3\2\2\2/\u00d5\3\2\2\2\61\u00d7\3\2\2\2\63\u00d9\3\2\2\2"+
		"\65\u00db\3\2\2\2\67\u00dd\3\2\2\29\u00df\3\2\2\2;\u00e1\3\2\2\2=\u00e3"+
		"\3\2\2\2?\u00e5\3\2\2\2A\u00e7\3\2\2\2C\u00e9\3\2\2\2E\u00eb\3\2\2\2G"+
		"\u00ed\3\2\2\2I\u00f0\3\2\2\2K\u00f3\3\2\2\2M\u00f5\3\2\2\2O\u00f7\3\2"+
		"\2\2Q\u00fa\3\2\2\2S\u00fc\3\2\2\2U\u0103\3\2\2\2W\u010b\3\2\2\2Y\u010f"+
		"\3\2\2\2[\u011d\3\2\2\2]\u012e\3\2\2\2_\u013f\3\2\2\2a\u0151\3\2\2\2c"+
		"\u015f\3\2\2\2e\u016e\3\2\2\2g\u017e\3\2\2\2i\u0183\3\2\2\2k\u018c\3\2"+
		"\2\2mo\t\2\2\2nm\3\2\2\2op\3\2\2\2pn\3\2\2\2pq\3\2\2\2qr\3\2\2\2rs\b\2"+
		"\2\2s\4\3\2\2\2tu\7e\2\2uv\7n\2\2vw\7c\2\2wx\7u\2\2xy\7u\2\2y\6\3\2\2"+
		"\2z{\7k\2\2{|\7p\2\2|}\7j\2\2}~\7g\2\2~\177\7t\2\2\177\u0080\7k\2\2\u0080"+
		"\u0081\7v\2\2\u0081\u0082\7u\2\2\u0082\b\3\2\2\2\u0083\u0084\7k\2\2\u0084"+
		"\u0085\7h\2\2\u0085\n\3\2\2\2\u0086\u0087\7v\2\2\u0087\u0088\7j\2\2\u0088"+
		"\u0089\7g\2\2\u0089\u008a\7p\2\2\u008a\f\3\2\2\2\u008b\u008c\7g\2\2\u008c"+
		"\u008d\7n\2\2\u008d\u008e\7u\2\2\u008e\u008f\7g\2\2\u008f\16\3\2\2\2\u0090"+
		"\u0091\7h\2\2\u0091\u0092\7k\2\2\u0092\20\3\2\2\2\u0093\u0094\7n\2\2\u0094"+
		"\u0095\7q\2\2\u0095\u0096\7q\2\2\u0096\u0097\7r\2\2\u0097\22\3\2\2\2\u0098"+
		"\u0099\7r\2\2\u0099\u009a\7q\2\2\u009a\u009b\7q\2\2\u009b\u009c\7n\2\2"+
		"\u009c\24\3\2\2\2\u009d\u009e\7y\2\2\u009e\u009f\7j\2\2\u009f\u00a0\7"+
		"k\2\2\u00a0\u00a1\7n\2\2\u00a1\u00a2\7g\2\2\u00a2\26\3\2\2\2\u00a3\u00a4"+
		"\7e\2\2\u00a4\u00a5\7c\2\2\u00a5\u00a6\7u\2\2\u00a6\u00a7\7g\2\2\u00a7"+
		"\30\3\2\2\2\u00a8\u00a9\7g\2\2\u00a9\u00aa\7u\2\2\u00aa\u00ab\7c\2\2\u00ab"+
		"\u00ac\7e\2\2\u00ac\32\3\2\2\2\u00ad\u00ae\7v\2\2\u00ae\u00af\7t\2\2\u00af"+
		"\u00b0\7w\2\2\u00b0\u00b1\7g\2\2\u00b1\34\3\2\2\2\u00b2\u00b3\7h\2\2\u00b3"+
		"\u00b4\7c\2\2\u00b4\u00b5\7n\2\2\u00b5\u00b6\7u\2\2\u00b6\u00b7\7g\2\2"+
		"\u00b7\36\3\2\2\2\u00b8\u00b9\7n\2\2\u00b9\u00ba\7g\2\2\u00ba\u00bb\7"+
		"v\2\2\u00bb \3\2\2\2\u00bc\u00bd\7k\2\2\u00bd\u00be\7p\2\2\u00be\"\3\2"+
		"\2\2\u00bf\u00c0\7k\2\2\u00c0\u00c1\7u\2\2\u00c1\u00c2\7x\2\2\u00c2\u00c3"+
		"\7q\2\2\u00c3\u00c4\7k\2\2\u00c4\u00c5\7f\2\2\u00c5$\3\2\2\2\u00c6\u00c7"+
		"\7p\2\2\u00c7\u00c8\7g\2\2\u00c8\u00c9\7y\2\2\u00c9&\3\2\2\2\u00ca\u00cb"+
		"\7q\2\2\u00cb\u00cc\7h\2\2\u00cc(\3\2\2\2\u00cd\u00ce\7p\2\2\u00ce\u00cf"+
		"\7q\2\2\u00cf\u00d0\7v\2\2\u00d0*\3\2\2\2\u00d1\u00d2\7}\2\2\u00d2,\3"+
		"\2\2\2\u00d3\u00d4\7\177\2\2\u00d4.\3\2\2\2\u00d5\u00d6\7*\2\2\u00d6\60"+
		"\3\2\2\2\u00d7\u00d8\7+\2\2\u00d8\62\3\2\2\2\u00d9\u00da\7=\2\2\u00da"+
		"\64\3\2\2\2\u00db\u00dc\7<\2\2\u00dc\66\3\2\2\2\u00dd\u00de\7.\2\2\u00de"+
		"8\3\2\2\2\u00df\u00e0\7-\2\2\u00e0:\3\2\2\2\u00e1\u00e2\7/\2\2\u00e2<"+
		"\3\2\2\2\u00e3\u00e4\7,\2\2\u00e4>\3\2\2\2\u00e5\u00e6\7\61\2\2\u00e6"+
		"@\3\2\2\2\u00e7\u00e8\7\u0080\2\2\u00e8B\3\2\2\2\u00e9\u00ea\7?\2\2\u00ea"+
		"D\3\2\2\2\u00eb\u00ec\7>\2\2\u00ecF\3\2\2\2\u00ed\u00ee\7>\2\2\u00ee\u00ef"+
		"\7?\2\2\u00efH\3\2\2\2\u00f0\u00f1\7>\2\2\u00f1\u00f2\7/\2\2\u00f2J\3"+
		"\2\2\2\u00f3\u00f4\7\60\2\2\u00f4L\3\2\2\2\u00f5\u00f6\7B\2\2\u00f6N\3"+
		"\2\2\2\u00f7\u00f8\7?\2\2\u00f8\u00f9\7@\2\2\u00f9P\3\2\2\2\u00fa\u00fb"+
		"\t\3\2\2\u00fbR\3\2\2\2\u00fc\u0100\t\4\2\2\u00fd\u00ff\t\5\2\2\u00fe"+
		"\u00fd\3\2\2\2\u00ff\u0102\3\2\2\2\u0100\u00fe\3\2\2\2\u0100\u0101\3\2"+
		"\2\2\u0101T\3\2\2\2\u0102\u0100\3\2\2\2\u0103\u0107\t\6\2\2\u0104\u0106"+
		"\t\5\2\2\u0105\u0104\3\2\2\2\u0106\u0109\3\2\2\2\u0107\u0105\3\2\2\2\u0107"+
		"\u0108\3\2\2\2\u0108V\3\2\2\2\u0109\u0107\3\2\2\2\u010a\u010c\t\7\2\2"+
		"\u010b\u010a\3\2\2\2\u010c\u010d\3\2\2\2\u010d\u010b\3\2\2\2\u010d\u010e"+
		"\3\2\2\2\u010eX\3\2\2\2\u010f\u0110\7/\2\2\u0110\u0111\7/\2\2\u0111\u0115"+
		"\3\2\2\2\u0112\u0114\13\2\2\2\u0113\u0112\3\2\2\2\u0114\u0117\3\2\2\2"+
		"\u0115\u0116\3\2\2\2\u0115\u0113\3\2\2\2\u0116\u0119\3\2\2\2\u0117\u0115"+
		"\3\2\2\2\u0118\u011a\t\b\2\2\u0119\u0118\3\2\2\2\u011a\u011b\3\2\2\2\u011b"+
		"\u011c\b-\2\2\u011cZ\3\2\2\2\u011d\u0128\7$\2\2\u011e\u011f\7^\2\2\u011f"+
		"\u0127\7$\2\2\u0120\u0121\7^\2\2\u0121\u0122\7\17\2\2\u0122\u0127\7\f"+
		"\2\2\u0123\u0124\7^\2\2\u0124\u0127\7\f\2\2\u0125\u0127\n\t\2\2\u0126"+
		"\u011e\3\2\2\2\u0126\u0120\3\2\2\2\u0126\u0123\3\2\2\2\u0126\u0125\3\2"+
		"\2\2\u0127\u012a\3\2\2\2\u0128\u0129\3\2\2\2\u0128\u0126\3\2\2\2\u0129"+
		"\u012b\3\2\2\2\u012a\u0128\3\2\2\2\u012b\u012c\7$\2\2\u012c\u012d\b.\3"+
		"\2\u012d\\\3\2\2\2\u012e\u0139\7$\2\2\u012f\u0130\7^\2\2\u0130\u0138\7"+
		"$\2\2\u0131\u0132\7^\2\2\u0132\u0133\7\17\2\2\u0133\u0138\7\f\2\2\u0134"+
		"\u0135\7^\2\2\u0135\u0138\7\f\2\2\u0136\u0138\n\n\2\2\u0137\u012f\3\2"+
		"\2\2\u0137\u0131\3\2\2\2\u0137\u0134\3\2\2\2\u0137\u0136\3\2\2\2\u0138"+
		"\u013b\3\2\2\2\u0139\u013a\3\2\2\2\u0139\u0137\3\2\2\2\u013a\u013c\3\2"+
		"\2\2\u013b\u0139\3\2\2\2\u013c\u013d\7\f\2\2\u013d\u013e\b/\4\2\u013e"+
		"^\3\2\2\2\u013f\u014b\7$\2\2\u0140\u014a\7\2\2\2\u0141\u0142\7^\2\2\u0142"+
		"\u014a\7$\2\2\u0143\u0144\7^\2\2\u0144\u0145\7\17\2\2\u0145\u014a\7\f"+
		"\2\2\u0146\u0147\7^\2\2\u0147\u014a\7\f\2\2\u0148\u014a\n\n\2\2\u0149"+
		"\u0140\3\2\2\2\u0149\u0141\3\2\2\2\u0149\u0143\3\2\2\2\u0149\u0146\3\2"+
		"\2\2\u0149\u0148\3\2\2\2\u014a\u014d\3\2\2\2\u014b\u014c\3\2\2\2\u014b"+
		"\u0149\3\2\2\2\u014c\u014e\3\2\2\2\u014d\u014b\3\2\2\2\u014e\u014f\7$"+
		"\2\2\u014f\u0150\b\60\5\2\u0150`\3\2\2\2\u0151\u0159\7$\2\2\u0152\u0153"+
		"\7^\2\2\u0153\u0158\7$\2\2\u0154\u0155\7^\2\2\u0155\u0158\7p\2\2\u0156"+
		"\u0158\n\n\2\2\u0157\u0152\3\2\2\2\u0157\u0154\3\2\2\2\u0157\u0156\3\2"+
		"\2\2\u0158\u015b\3\2\2\2\u0159\u015a\3\2\2\2\u0159\u0157\3\2\2\2\u015a"+
		"\u015c\3\2\2\2\u015b\u0159\3\2\2\2\u015c\u015d\7\2\2\3\u015d\u015e\b\61"+
		"\6\2\u015eb\3\2\2\2\u015f\u0160\7*\2\2\u0160\u0161\7,\2\2\u0161\u0166"+
		"\3\2\2\2\u0162\u0165\5c\62\2\u0163\u0165\13\2\2\2\u0164\u0162\3\2\2\2"+
		"\u0164\u0163\3\2\2\2\u0165\u0168\3\2\2\2\u0166\u0167\3\2\2\2\u0166\u0164"+
		"\3\2\2\2\u0167\u0169\3\2\2\2\u0168\u0166\3\2\2\2\u0169\u016a\7,\2\2\u016a"+
		"\u016b\7+\2\2\u016b\u016c\3\2\2\2\u016c\u016d\b\62\2\2\u016dd\3\2\2\2"+
		"\u016e\u016f\7*\2\2\u016f\u0170\7,\2\2\u0170\u0175\3\2\2\2\u0171\u0174"+
		"\5c\62\2\u0172\u0174\13\2\2\2\u0173\u0171\3\2\2\2\u0173\u0172\3\2\2\2"+
		"\u0174\u0177\3\2\2\2\u0175\u0176\3\2\2\2\u0175\u0173\3\2\2\2\u0176\u017c"+
		"\3\2\2\2\u0177\u0175\3\2\2\2\u0178\u0179\7,\2\2\u0179\u017d\7+\2\2\u017a"+
		"\u017b\7\2\2\3\u017b\u017d\b\63\7\2\u017c\u0178\3\2\2\2\u017c\u017a\3"+
		"\2\2\2\u017df\3\2\2\2\u017e\u017f\7,\2\2\u017f\u0180\7+\2\2\u0180\u0181"+
		"\3\2\2\2\u0181\u0182\b\64\b\2\u0182h\3\2\2\2\u0183\u0184\n\13\2\2\u0184"+
		"\u0185\b\65\t\2\u0185j\3\2\2\2\u0186\u018d\5e\63\2\u0187\u018d\5g\64\2"+
		"\u0188\u018d\5]/\2\u0189\u018d\5_\60\2\u018a\u018d\5a\61\2\u018b\u018d"+
		"\5i\65\2\u018c\u0186\3\2\2\2\u018c\u0187\3\2\2\2\u018c\u0188\3\2\2\2\u018c"+
		"\u0189\3\2\2\2\u018c\u018a\3\2\2\2\u018c\u018b\3\2\2\2\u018dl\3\2\2\2"+
		"\27\2p\u0100\u0107\u010d\u0115\u0119\u0126\u0128\u0137\u0139\u0149\u014b"+
		"\u0157\u0159\u0164\u0166\u0173\u0175\u017c\u018c\n\b\2\2\3.\2\3/\3\3\60"+
		"\4\3\61\5\3\63\6\3\64\7\3\65\b";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}