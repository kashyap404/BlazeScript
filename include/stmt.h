#pragma once
#include "expr.h"
#include "type.h"
#include "visitor.h"
#include <memory>
#include <utility>
#include <vector>

class Stmt {
public:
    Stmt(int line, int column) : line(line), column(column) {}
    virtual ~Stmt() = default;
    int line;
    int column;

    virtual void accept(Visitor& visitor) = 0;
};

class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;

    ExpressionStmt(std::unique_ptr<Expr> expr, int line, int column)
        : Stmt(line, column), expression(std::move(expr)) {}

    void accept(Visitor& visitor) override { visitor.visitExpressionStmt(*this); }
};

class ReturnStmt : public Stmt {
public:
    std::unique_ptr<Expr> value;
    ReturnStmt(std::unique_ptr<Expr> val, int line, int column)
        : Stmt(line, column), value(std::move(val)) {}

    void accept(Visitor& visitor) override { visitor.visitReturnStmt(*this); }
};

class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> stmt, int line, int column)
        : Stmt(line, column), condition(std::move(cond)), body(std::move(stmt)) {}

    void accept(Visitor& visitor) override { visitor.visitWhileStmt(*this); }
};

class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> thenStmt,
           std::unique_ptr<Stmt> elseStmt, int line, int column)
        : Stmt(line, column), condition(std::move(cond)), thenBranch(std::move(thenStmt)),
          elseBranch(std::move(elseStmt)) {}

    void accept(Visitor& visitor) override { visitor.visitIfStmt(*this); }
};

class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts, int line, int column)
        : Stmt(line, column), statements(std::move(stmts)) {}

    void accept(Visitor& visitor) override { visitor.visitBlockStmt(*this); }
};
class VarDeclStmt : public Stmt {
public:
    Token name;
    Type* type;
    std::unique_ptr<Expr> initializer;

    VarDeclStmt(Token n, Type* t, std::unique_ptr<Expr> init)
        : Stmt(n.line_, n.column_), name(std::move(n)), type(t), initializer(std::move(init)) {}

    void accept(Visitor& visitor) override { visitor.visitVarDeclStmt(*this); }
};