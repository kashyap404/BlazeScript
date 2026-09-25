#pragma once

#include "expr.h"
#include "function.h"
#include "stmt.h"
#include "visitor.h"
#include "parser.h" 
#include <iostream>

class AstPrinter : public Visitor {
private:
    std::ostream* out_;
    int depth_ = 0;

    void indent();
    void emit(const char* text);
    void emitOperator(OperatorType op);
    void printFuncDefn(const FuncDefn& fn);

public:
    void print(Expr& expr, std::ostream& out);
    void print(Stmt& stmt, std::ostream& out);
    void print(const FuncDefn& fn, std::ostream& out);
    void print(const Program& program, std::ostream& out);

    // Expressions
    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitVariableExpr(VariableExpr& expr) override;
    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;
    void visitAssignmentExpr(AssignmentExpr& expr) override;
    void visitCallExpr(CallExpr& expr) override;
    void visitArrayLiteralExpr(ArrayLiteralExpr& expr) override;
    void visitIndexExpr(IndexExpr& expr) override;

    // Statements
    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitIfStmt(IfStmt& stmt) override;
    void visitWhileStmt(WhileStmt& stmt) override;
    void visitReturnStmt(ReturnStmt& stmt) override;
    void visitBlockStmt(BlockStmt& stmt) override;
    void visitVarDeclStmt(VarDeclStmt& stmt) override;
};