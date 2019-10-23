package cool.parser;


import java.util.List;

import cool.AST.*;
import cool.parser.CoolParser.*;
import cool.structures.SymbolTable;


public class MyParseTreeVisitor extends CoolParserBaseVisitor<ASTNode> {
	
	@Override
	public ASTNode visitProgram(ProgramContext ctx) {
		ProgNode p = new ProgNode();
		ClassNode c = null;		
		
		for(Class_defContext classCtx : ctx.classes)
		{
			String errStr = ClassNode.checkName(classCtx);
			
			if(errStr != null)
				SymbolTable.error(classCtx, classCtx.name, errStr);
			else
			{
				c = (ClassNode) this.visitClass_def(classCtx);
				p.addChild(c);
			}
			
		}
		p.populateScope();   								//add the classNames to the global scope
		p.createChildrenScopes();   						//create the scopes corresponding to the classes
		
		List<ClassNode> children = p.getChildren();
		for(ClassNode node : children)
		{
			if(node.getParToken() != null)
			{
				String parName = node.getParToken().getText();
				ClassNode parNode = p.getChild(parName);			//get the AST Node corresponding to the parent name
				if(parNode != null)
				{
					node.setParAttrScope(parNode.getAttrScope());	//now the parent scope is the the ASTNode parent's scope
					node.setParMethScope(parNode.getMethScope());	//not the global scope
				}
			}
		}
		
		for(ClassNode node : children)
		{
			node.checkParentScope();
			node.checkAttributes();			//check for redefined inherited attributes and attributes with undefined type
		}
		
		for(ClassNode node : children)
		{
			node.createChildrenMethScopes();
		}
		
		return p;
	};
	
	@Override
	public ASTNode visitClass_def(Class_defContext ctx) {
		ClassNode n = null;
		ASTNode f = null;
		
		n = new ClassNode(ctx);
		
		if(!ctx.features.isEmpty())
			for(FeatureContext feat : ctx.features)
			{
				f = feat.accept(this);
				
				if(f instanceof MethNode)
					n.addMeth((MethNode)f);
				if(f instanceof AttrNode)
					n.addAttr((AttrNode)f);
				
				n.addFeat(f);
			}
		return n;
	}
	
	@Override
	public ASTNode visitAttr(AttrContext ctx) {
		AttrNode a = new AttrNode(ctx, 1);
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
		MethNode m = new MethNode(ctx);
		FormalNode f = null;
		
		ASTNode body = ctx.body.accept(this);
		m.setBody(body);
		
		if(!ctx.params.isEmpty())
			for(FormalContext form : ctx.params)
			{
				f = new FormalNode(form);
				m.addParam(f);
			}
		return m;
	}
	
	@Override
	public ASTNode visitDiv(DivContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left.start, ctx.right.start);
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitMul(MulContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left.start, ctx.right.start);
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitAdd(AddContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left.start, ctx.right.start);
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitSubst(SubstContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left.start, ctx.right.start);
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitLess_or_eq(Less_or_eqContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left.start, ctx.right.start);
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitLess(LessContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left.start, ctx.right.start);
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitEq(EqContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left.start, ctx.right.start);
		ASTNode left = ctx.left.accept(this);
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitAssign(AssignContext ctx) {
		BinaryOpNode o = new BinaryOpNode(ctx, ctx.op, ctx.left, ctx.right.start);
		ASTNode left = new LiteralNode(ctx, ctx.left, "classType");
		ASTNode right = ctx.right.accept(this);

		o.addOperands(left, right);
		return o;
	}
	
	@Override
	public ASTNode visitCompl(ComplContext ctx) {
		UnaryOpNode u = new UnaryOpNode(ctx, ctx.op);
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitNot(NotContext ctx) {
		UnaryOpNode u = new UnaryOpNode(ctx, ctx.op);
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitBracket_expr(Bracket_exprContext ctx) {
		UnaryOpNode u = new UnaryOpNode(ctx, ctx.start);
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitIsvoid(IsvoidContext ctx) {
		UnaryOpNode u = new UnaryOpNode(ctx, ctx.op);
		ASTNode operand = ctx.exp.accept(this);
	
		u.addOperand(operand);
		return u;
	}
	
	@Override
	public ASTNode visitNew(NewContext ctx) {
		return new NewOpNode(ctx, ctx.name);
	}
	
	@Override
	public ASTNode visitId(IdContext ctx) {
		return new LiteralNode(ctx, ctx.start, "classType");
	}
	
	@Override
	public ASTNode visitNum(NumContext ctx) {
		return new LiteralNode(ctx, ctx.start, "Int");
	}
	
	@Override
	public ASTNode visitFalse(FalseContext ctx) {
		return new LiteralNode(ctx, ctx.start, "Bool");
	}
	
	@Override
	public ASTNode visitTrue(TrueContext ctx) {
		return new LiteralNode(ctx, ctx.start, "Bool");
	}
	
	@Override
	public ASTNode visitStr(StrContext ctx) {
		return new StrNode(ctx, ctx.start, "String");
	}
	
	@Override
	public ASTNode visitIf_instr(If_instrContext ctx) {
		IfNode i = new IfNode(ctx);
		
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

		o.addOperands(ctx, cond, expr);
		return o;
	}
	
	@Override
	public ASTNode visitFun_call(Fun_callContext ctx) {
		LiteralNode methName = new LiteralNode(ctx, ctx.name, "classType");
		ChildrenListNode c = new ChildrenListNode(ctx,"implicit dispatch", null, null, methName);
		ASTNode n = null;
		
		for(ExprContext e : ctx.values)
		{
			n = e.accept(this);
			c.addChild(n);
		}
		
		return c;
	}
	
	@Override
	public ASTNode visitMemb_fun_call(Memb_fun_callContext ctx) {
		LiteralNode actualCName, methName;
		ChildrenListNode f;
		ASTNode n;

		ASTNode className = ctx.exp.accept(this);
		
		methName = new LiteralNode(ctx, ctx.name, "classType");
		if(ctx.type != null)
			actualCName = new LiteralNode(ctx, ctx.type, "classType");
		else
			actualCName = null;
		
		f = new ChildrenListNode(ctx, ".", className, actualCName, methName);
		
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

		b = new ChildrenListNode(ctx, "block", null, null, null);
		
		for(ExprContext e : ctx.values)
		{
			n = e.accept(this);
			b.addChild(n);
		}
		
		return b;
	}
	
	@Override
	public ASTNode visitAttrib(AttribContext ctx) {
		AttrNode a = new AttrNode(ctx, 2);
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
		LetInstrNode l = new LetInstrNode(ctx, expr);

		for(AttribContext a : ctx.params)
		{
			attrib = (AttrNode) a.accept(this);
			l.addParam(attrib);
		}
			
		return l;
	}
	
	@Override
	public ASTNode visitCase_attrib(Case_attribContext ctx) {
		AttrNode a = new AttrNode(ctx, 3);
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
		CaseOpNode c = new CaseOpNode(expr, ctx);
		
		for(Case_attribContext a : ctx.values)
		{
			attrib = (AttrNode) a.accept(this);
			c.addParam(attrib);
		}
		
		return c;
	}
	
}

