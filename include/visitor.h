#pragma once

class LiteralExpr;
class VariableExpr;
class BinaryExpr;
class UnaryExpr;
class AssignmentExpr;
class CallExpr;


class ExpressionStmt;
class IfStmt;
class WhileStmt;
class ReturnStmt;
class BlockStmt;

class Visitor {
public:
    virtual ~Visitor() = default;

    // Expressions
    virtual void visitLiteralExpr(LiteralExpr& expr) = 0;
    virtual void visitVariableExpr(VariableExpr& expr) = 0;
    virtual void visitBinaryExpr(BinaryExpr& expr) = 0;
    virtual void visitUnaryExpr(UnaryExpr& expr) = 0;
    virtual void visitAssignmentExpr(AssignmentExpr& expr) = 0;
    virtual void visitCallExpr(CallExpr& expr) = 0;

    // Statements
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitIfStmt(IfStmt& stmt) = 0;
    virtual void visitWhileStmt(WhileStmt& stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt& stmt) = 0;
    virtual void visitBlockStmt(BlockStmt& stmt) = 0;
};