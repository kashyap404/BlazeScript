#pragma once

#include "expr.h"
#include "function.h"
#include "scope.h"
#include "stmt.h"
#include "parser.h" 
#include "type.h"
#include "visitor.h"
#include <string>
#include <unordered_map>

class SemanticAnalyzer : public Visitor {
private:
    SymbolTable symbols_;
    TypeTable types_;
    Type* lastType_ = nullptr;
    Type* currentFunctionReturnType_ = nullptr;
    bool hadError_ = false;
    std::unordered_map<std::string, FuncDefn*> functions_;

    void error(int line, int column, const std::string& message);
    void analyzeFunction(FuncDefn& fn);

public:
    SemanticAnalyzer();
    void analyze(Program& program);
    bool hadError() const { return hadError_; }

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