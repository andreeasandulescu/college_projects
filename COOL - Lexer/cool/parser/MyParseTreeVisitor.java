package cool.parser;

import cool.AST.*;
import cool.parser.CoolParser.*;

public class MyParseTreeVisitor extends CoolParserBaseVisitor<ASTNode> {
	
	
	@Override
	public ASTNode visitProgram(ProgramContext ctx) {
		ProgNode p = new ProgNode();
		ClassNode c = null;
		
		for(Class_defContext clss : ctx.classes)
		{
			c = (ClassNode) this.visitClass_def(clss);
			p.addChild(c);
		}
		return p;
	};
	
	@Override
	public ASTNode visitClass_def(Class_defContext ctx) {
		ClassNode n = null;
		ASTNode f = null;
		
		if (ctx.parName != null)
			n = new ClassNode(ctx.name.getText(), ctx.parName.getText());
		else
			n = new ClassNode(ctx.name.getText());
		
		if(!ctx.features.isEmpty())
			for(FeatureContext feat : ctx.features)
			{
				f = feat.accept(this);
				n.addFeat(f);
			}
		return n;
	}
	
	@Override
	public ASTNode visitAttr(AttrContext ctx) {
		AttrNode a = new AttrNode(ctx.name.getText(), ctx.type.getText(), 1);
		ASTNode expr = null;
		
		if(ctx.val != null)
		{
			expr = ctx.val.accept(this);
			a.setExpr(expr);
		}
		return a;
	}
	
	@Override
	public ASTNode visitMeth(MethContext ctx) {
		MethNode m = new MethNode(ctx.name.getText(), ctx.type.getText());
		Formal f = null;
		
		ASTNode body = ctx.body.accept(this);
		m.setBody(body);
		
		if(!ctx.params.isEmpty())
			for(FormalContext form : ctx.params)
			{
				f = new Formal(form.name.getText(), form.type.getText());
				m.addParam(f);
			}
		
		return m;
	}
	
	@Override
	public ASTNode visitDiv(DivContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitMul(MulContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitAdd(AddContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());	
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitSubst(SubstContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());	
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitLess_or_eq(Less_or_eqContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());	
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitLess(LessContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());	
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitEq(EqContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());	
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitAssign(AssignContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx.op.getText());	
		ASTNode left = new LiteralNode(ctx.left.getText());
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitCompl(ComplContext ctx) {
		UnaryOpNode u = new UnaryOpNode(ctx.op.getText());
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitNot(NotContext ctx) {
		UnaryOpNode u = new UnaryOpNode(ctx.op.getText());
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitBracket_expr(Bracket_exprContext ctx) {
		UnaryOpNode u = new UnaryOpNode("");
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitIsvoid(IsvoidContext ctx) {
		UnaryOpNode u = new UnaryOpNode(ctx.op.getText());
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitNew(NewContext ctx) {
		return new NewOpNode(ctx.name.getText());
	}
	
	@Override
	public ASTNode visitId(IdContext ctx) {
		return new LiteralNode(ctx.getText());
	}
	
	@Override
	public ASTNode visitNum(NumContext ctx) {
		return new LiteralNode(ctx.getText());
	}
	
	@Override
	public ASTNode visitFalse(FalseContext ctx) {
		return new LiteralNode(ctx.getText());
	}
	
	@Override
	public ASTNode visitStr(StrContext ctx) {
		return new StrNode(ctx.getText());
	}
	
	@Override
	public ASTNode visitIf_instr(If_instrContext ctx) {
		IfNode i = new IfNode();
		
		ASTNode ifExpr = ctx.if_e.accept(this);
		ASTNode thenExpr = ctx.then_e.accept(this);
		ASTNode elseExpr = ctx.else_e.accept(this);
		
		i.addExpressions(ifExpr, thenExpr, elseExpr);
		
		return i;
	}
	
	@Override
	public ASTNode visitWhile_instr(While_instrContext ctx) {
		WhileNode o = new WhileNode();
		ASTNode cond = ctx.while_e.accept(this);
		ASTNode expr = ctx.loop_e.accept(this);

		o.addOperands(cond, expr);
		return o;
	}
	
	@Override
	public ASTNode visitFun_call(Fun_callContext ctx) {
		LiteralNode methName = new LiteralNode(ctx.name.getText());
		ChildrenListNode c = new ChildrenListNode("implicit dispatch", null, null, methName);
		ASTNode n = null;
		
		for(ExprContext e : ctx.values)			//method parameters
		{
			n = e.accept(this);
			c.addChild(n);
		}
		
		return c;
	}
	
	@Override
	public ASTNode visitMemb_fun_call(Memb_fun_callContext ctx) {
		LiteralNode actualCName, methName;
		ASTNode n;

		ASTNode objName = ctx.exp.accept(this);
		
		methName = new LiteralNode(ctx.name.getText());
		if(ctx.type != null)
			actualCName = new LiteralNode(ctx.type.getText());
		else
			actualCName = null;
		
		ChildrenListNode f = new ChildrenListNode(".", objName, actualCName, methName);
		
		for(ExprContext e : ctx.values)
		{
			n = e.accept(this);
			f.addChild(n);
		}
		
		return f;
	}
	
	@Override
	public ASTNode visitBlock(BlockContext ctx) {
		ChildrenListNode b;
		ASTNode n;

		b = new ChildrenListNode("block", null, null, null);
		
		for(ExprContext e : ctx.values)
		{
			n = e.accept(this);
			b.addChild(n);
		}
		
		return b;
	}
	
	@Override
	public ASTNode visitAttrib(AttribContext ctx) {
		AttrNode a = new AttrNode(ctx.name.getText(), ctx.type.getText(), 2);
		ASTNode expr = null;
		
		if(ctx.val != null)
		{
			expr = ctx.val.accept(this);
			a.setExpr(expr);
		}
		return a;
	}
	
	@Override
	public ASTNode visitLet_instr(Let_instrContext ctx) {
		AttrNode attrib;
		ASTNode expr = ctx.exp.accept(this);
		LetInstrNode l = new LetInstrNode(expr);
		
		for(AttribContext a : ctx.params)
		{
			attrib = (AttrNode) a.accept(this);
			l.addParam(attrib);
		}
			
		return l;
	}
	
	@Override
	public ASTNode visitCase_attrib(Case_attribContext ctx) {
		AttrNode a = new AttrNode(ctx.name.getText(), ctx.type.getText(), 3);
		ASTNode expr = null;
		
		if(ctx.val != null)
		{
			expr = ctx.val.accept(this);
			a.setExpr(expr);
		}
		return a;
	}
	
	@Override
	public ASTNode visitCase_instr(Case_instrContext ctx) {
		AttrNode attrib;
		ASTNode expr = ctx.exp.accept(this);
		CaseOpNode c = new CaseOpNode(expr);
		
		for(Case_attribContext a : ctx.values)
		{
			attrib = (AttrNode) a.accept(this);
			c.addParam(attrib);
		}
		
		return c;
	}
	
}

