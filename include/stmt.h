#pragma once 
#include <memory>
#include <vector>
#include <utility>
#include <optional>
#include "expr.h"
#include "token.h"

class Stmt
{
    public:
    Stmt(int line, int column)
        : line(line), column(column) {}
    virtual ~Stmt() = default;
    int line;
    int column;
};

class ExpressionStmt : public Stmt
{
    public:
    std::unique_ptr<Expr> expression;

    ExpressionStmt(std::unique_ptr<Expr> expr, int line, int column)
        : Stmt(line, column),
          expression(std::move(expr)) {}
};

class ReturnStmt : public Stmt
{
    public:
    std::unique_ptr<Expr> value;
    ReturnStmt(std::unique_ptr<Expr> val, int line, int column)
        : Stmt(line, column),
          value(std::move(val)) {}
};

class WhileStmt : public Stmt
{
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> cond,
              std::unique_ptr<Stmt> stmt,
              int line,
              int column)
        : Stmt(line, column),
          condition(std::move(cond)),
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
           std::unique_ptr<Stmt> elseStmt,
           int line,
           int column)
        : Stmt(line, column),
          condition(std::move(cond)),
          thenBranch(std::move(thenStmt)),
          elseBranch(std::move(elseStmt)) {}
};

class BlockStmt : public Stmt
{
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts, int line, int column)
        : Stmt(line, column),
          statements(std::move(stmts)) {}
};
struct Param
{
    Token name;
};

class FunctionStmt : public Stmt
{
public:
    Token name;
    std::vector<Param> params;
    std::unique_ptr<BlockStmt> body;

    FunctionStmt(Token name,
                 std::vector<Param> params,
                 std::unique_ptr<BlockStmt> body,
                 int line,
                 int column)
        : Stmt(line, column),
          name(std::move(name)),
          params(std::move(params)),
          body(std::move(body)) {}
};