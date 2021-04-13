// Generated from CoolParser.g4 by ANTLR 4.7.1

    package cool.parser;

import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link CoolParser}.
 */
public interface CoolParserListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link CoolParser#program}.
	 * @param ctx the parse tree
	 */
	void enterProgram(CoolParser.ProgramContext ctx);
	/**
	 * Exit a parse tree produced by {@link CoolParser#program}.
	 * @param ctx the parse tree
	 */
	void exitProgram(CoolParser.ProgramContext ctx);
	/**
	 * Enter a parse tree produced by {@link CoolParser#class_def}.
	 * @param ctx the parse tree
	 */
	void enterClass_def(CoolParser.Class_defContext ctx);
	/**
	 * Exit a parse tree produced by {@link CoolParser#class_def}.
	 * @param ctx the parse tree
	 */
	void exitClass_def(CoolParser.Class_defContext ctx);
	/**
	 * Enter a parse tree produced by the {@code meth}
	 * labeled alternative in {@link CoolParser#feature}.
	 * @param ctx the parse tree
	 */
	void enterMeth(CoolParser.MethContext ctx);
	/**
	 * Exit a parse tree produced by the {@code meth}
	 * labeled alternative in {@link CoolParser#feature}.
	 * @param ctx the parse tree
	 */
	void exitMeth(CoolParser.MethContext ctx);
	/**
	 * Enter a parse tree produced by the {@code attr}
	 * labeled alternative in {@link CoolParser#feature}.
	 * @param ctx the parse tree
	 */
	void enterAttr(CoolParser.AttrContext ctx);
	/**
	 * Exit a parse tree produced by the {@code attr}
	 * labeled alternative in {@link CoolParser#feature}.
	 * @param ctx the parse tree
	 */
	void exitAttr(CoolParser.AttrContext ctx);
	/**
	 * Enter a parse tree produced by {@link CoolParser#formal}.
	 * @param ctx the parse tree
	 */
	void enterFormal(CoolParser.FormalContext ctx);
	/**
	 * Exit a parse tree produced by {@link CoolParser#formal}.
	 * @param ctx the parse tree
	 */
	void exitFormal(CoolParser.FormalContext ctx);
	/**
	 * Enter a parse tree produced by {@link CoolParser#attrib}.
	 * @param ctx the parse tree
	 */
	void enterAttrib(CoolParser.AttribContext ctx);
	/**
	 * Exit a parse tree produced by {@link CoolParser#attrib}.
	 * @param ctx the parse tree
	 */
	void exitAttrib(CoolParser.AttribContext ctx);
	/**
	 * Enter a parse tree produced by {@link CoolParser#case_attrib}.
	 * @param ctx the parse tree
	 */
	void enterCase_attrib(CoolParser.Case_attribContext ctx);
	/**
	 * Exit a parse tree produced by {@link CoolParser#case_attrib}.
	 * @param ctx the parse tree
	 */
	void exitCase_attrib(CoolParser.Case_attribContext ctx);
	/**
	 * Enter a parse tree produced by the {@code mul}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterMul(CoolParser.MulContext ctx);
	/**
	 * Exit a parse tree produced by the {@code mul}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitMul(CoolParser.MulContext ctx);
	/**
	 * Enter a parse tree produced by the {@code isvoid}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterIsvoid(CoolParser.IsvoidContext ctx);
	/**
	 * Exit a parse tree produced by the {@code isvoid}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitIsvoid(CoolParser.IsvoidContext ctx);
	/**
	 * Enter a parse tree produced by the {@code num}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterNum(CoolParser.NumContext ctx);
	/**
	 * Exit a parse tree produced by the {@code num}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitNum(CoolParser.NumContext ctx);
	/**
	 * Enter a parse tree produced by the {@code div}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterDiv(CoolParser.DivContext ctx);
	/**
	 * Exit a parse tree produced by the {@code div}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitDiv(CoolParser.DivContext ctx);
	/**
	 * Enter a parse tree produced by the {@code fun_call}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterFun_call(CoolParser.Fun_callContext ctx);
	/**
	 * Exit a parse tree produced by the {@code fun_call}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitFun_call(CoolParser.Fun_callContext ctx);
	/**
	 * Enter a parse tree produced by the {@code not}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterNot(CoolParser.NotContext ctx);
	/**
	 * Exit a parse tree produced by the {@code not}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitNot(CoolParser.NotContext ctx);
	/**
	 * Enter a parse tree produced by the {@code block}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterBlock(CoolParser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by the {@code block}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitBlock(CoolParser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by the {@code compl}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterCompl(CoolParser.ComplContext ctx);
	/**
	 * Exit a parse tree produced by the {@code compl}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitCompl(CoolParser.ComplContext ctx);
	/**
	 * Enter a parse tree produced by the {@code id}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterId(CoolParser.IdContext ctx);
	/**
	 * Exit a parse tree produced by the {@code id}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitId(CoolParser.IdContext ctx);
	/**
	 * Enter a parse tree produced by the {@code bracket_expr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterBracket_expr(CoolParser.Bracket_exprContext ctx);
	/**
	 * Exit a parse tree produced by the {@code bracket_expr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitBracket_expr(CoolParser.Bracket_exprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code less_or_eq}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterLess_or_eq(CoolParser.Less_or_eqContext ctx);
	/**
	 * Exit a parse tree produced by the {@code less_or_eq}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitLess_or_eq(CoolParser.Less_or_eqContext ctx);
	/**
	 * Enter a parse tree produced by the {@code add}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterAdd(CoolParser.AddContext ctx);
	/**
	 * Exit a parse tree produced by the {@code add}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitAdd(CoolParser.AddContext ctx);
	/**
	 * Enter a parse tree produced by the {@code new}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterNew(CoolParser.NewContext ctx);
	/**
	 * Exit a parse tree produced by the {@code new}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitNew(CoolParser.NewContext ctx);
	/**
	 * Enter a parse tree produced by the {@code memb_fun_call}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterMemb_fun_call(CoolParser.Memb_fun_callContext ctx);
	/**
	 * Exit a parse tree produced by the {@code memb_fun_call}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitMemb_fun_call(CoolParser.Memb_fun_callContext ctx);
	/**
	 * Enter a parse tree produced by the {@code if_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterIf_instr(CoolParser.If_instrContext ctx);
	/**
	 * Exit a parse tree produced by the {@code if_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitIf_instr(CoolParser.If_instrContext ctx);
	/**
	 * Enter a parse tree produced by the {@code let_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterLet_instr(CoolParser.Let_instrContext ctx);
	/**
	 * Exit a parse tree produced by the {@code let_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitLet_instr(CoolParser.Let_instrContext ctx);
	/**
	 * Enter a parse tree produced by the {@code false}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterFalse(CoolParser.FalseContext ctx);
	/**
	 * Exit a parse tree produced by the {@code false}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitFalse(CoolParser.FalseContext ctx);
	/**
	 * Enter a parse tree produced by the {@code subst}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterSubst(CoolParser.SubstContext ctx);
	/**
	 * Exit a parse tree produced by the {@code subst}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitSubst(CoolParser.SubstContext ctx);
	/**
	 * Enter a parse tree produced by the {@code less}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterLess(CoolParser.LessContext ctx);
	/**
	 * Exit a parse tree produced by the {@code less}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitLess(CoolParser.LessContext ctx);
	/**
	 * Enter a parse tree produced by the {@code eq}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterEq(CoolParser.EqContext ctx);
	/**
	 * Exit a parse tree produced by the {@code eq}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitEq(CoolParser.EqContext ctx);
	/**
	 * Enter a parse tree produced by the {@code str}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterStr(CoolParser.StrContext ctx);
	/**
	 * Exit a parse tree produced by the {@code str}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitStr(CoolParser.StrContext ctx);
	/**
	 * Enter a parse tree produced by the {@code case_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterCase_instr(CoolParser.Case_instrContext ctx);
	/**
	 * Exit a parse tree produced by the {@code case_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitCase_instr(CoolParser.Case_instrContext ctx);
	/**
	 * Enter a parse tree produced by the {@code true}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterTrue(CoolParser.TrueContext ctx);
	/**
	 * Exit a parse tree produced by the {@code true}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitTrue(CoolParser.TrueContext ctx);
	/**
	 * Enter a parse tree produced by the {@code while_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterWhile_instr(CoolParser.While_instrContext ctx);
	/**
	 * Exit a parse tree produced by the {@code while_instr}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitWhile_instr(CoolParser.While_instrContext ctx);
	/**
	 * Enter a parse tree produced by the {@code assign}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterAssign(CoolParser.AssignContext ctx);
	/**
	 * Exit a parse tree produced by the {@code assign}
	 * labeled alternative in {@link CoolParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitAssign(CoolParser.AssignContext ctx);
	/**
	 * Enter a parse tree produced by the {@code unterm_str}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void enterUnterm_str(CoolParser.Unterm_strContext ctx);
	/**
	 * Exit a parse tree produced by the {@code unterm_str}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void exitUnterm_str(CoolParser.Unterm_strContext ctx);
	/**
	 * Enter a parse tree produced by the {@code null_in_str}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void enterNull_in_str(CoolParser.Null_in_strContext ctx);
	/**
	 * Exit a parse tree produced by the {@code null_in_str}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void exitNull_in_str(CoolParser.Null_in_strContext ctx);
	/**
	 * Enter a parse tree produced by the {@code eof_in_str}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void enterEof_in_str(CoolParser.Eof_in_strContext ctx);
	/**
	 * Exit a parse tree produced by the {@code eof_in_str}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void exitEof_in_str(CoolParser.Eof_in_strContext ctx);
	/**
	 * Enter a parse tree produced by the {@code unmatch_comm}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void enterUnmatch_comm(CoolParser.Unmatch_commContext ctx);
	/**
	 * Exit a parse tree produced by the {@code unmatch_comm}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void exitUnmatch_comm(CoolParser.Unmatch_commContext ctx);
	/**
	 * Enter a parse tree produced by the {@code err_multi_comm}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void enterErr_multi_comm(CoolParser.Err_multi_commContext ctx);
	/**
	 * Exit a parse tree produced by the {@code err_multi_comm}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void exitErr_multi_comm(CoolParser.Err_multi_commContext ctx);
	/**
	 * Enter a parse tree produced by the {@code err_char}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void enterErr_char(CoolParser.Err_charContext ctx);
	/**
	 * Exit a parse tree produced by the {@code err_char}
	 * labeled alternative in {@link CoolParser#error}.
	 * @param ctx the parse tree
	 */
	void exitErr_char(CoolParser.Err_charContext ctx);
}