#pragma once 
#include <memory>
#include<vector>
#include <utility>

class Expr;
class Stmt
{
    public:
     virtual ~Stmt() = default;
};
class ExpressionStmt: public Stmt
{
    public:
    std::unique_ptr<Expr> expression;
    ExpressionStmt(std::unique_ptr<Expr> expr) 
        : expression(std::move(expr)) {}
};
class ReturnStmt:public Stmt
{
    public:
    std::unique_ptr<Expr> value;
    
    ReturnStmt(std::unique_ptr<Expr> val)
        : value(std::move(val)) {}
};
class WhileStmt : public Stmt
{
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> cond,
              std::unique_ptr<Stmt> stmt)
        : condition(std::move(cond)),
          body(std::move(stmt)) {}
};
class IfStmt : public Stmt
{
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> cond,
           std::unique_ptr<Stmt> thenStmt,
           std::unique_ptr<Stmt> elseStmt = nullptr)
        : condition(std::move(cond)),
          thenBranch(std::move(thenStmt)),
          elseBranch(std::move(elseStmt)) {}
};
class BlockStmt : public Stmt
{
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts)
        : statements(std::move(stmts)) {}
};  