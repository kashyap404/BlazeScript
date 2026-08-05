#pragma once
#include "visitor.h"
#include "expr.h"
#include "stmt.h"
#include "function.h"
#include "parser.h"
#include "type.h"
#include "scope.h"
#include "symbol.h"
#include <string>

class SemanticAnalyzer : public Visitor {
public:
    SemanticAnalyzer();

    void analyze(Program& program);
    bool hadError() const { return hadError_; }

    void visitLiteralExpr(LiteralExpr& expr) override;
    void visitVariableExpr(VariableExpr& expr) override;
    void visitBinaryExpr(BinaryExpr& expr) override;
    void visitUnaryExpr(UnaryExpr& expr) override;
    void visitAssignmentExpr(AssignmentExpr& expr) override;
    void visitCallExpr(CallExpr& expr) override;

    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitIfStmt(IfStmt& stmt) override;
    void visitWhileStmt(WhileStmt& stmt) override;
    void visitReturnStmt(ReturnStmt& stmt) override;
    void visitBlockStmt(BlockStmt& stmt) override;

private:
    void analyzeFunction(FuncDefn& fn);
    void error(int line, int column, const std::string& message);
    Type* lastType_ = nullptr;

    SymbolTable symbols_;
    TypeArena types_;
    Type* currentFunctionReturnType_ = nullptr;
    bool hadError_ = false;
};