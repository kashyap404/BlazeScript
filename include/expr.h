#pragma once
#include <memory>
#include "token.h"

enum class OperatorType {
    PLUS, MINUS, STAR, SLASH,
    BANG, AND, OR,
    EQUAL_EQUAL, BANG_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL
};

OperatorType tokenTypeToOperator(TokenType type);

class Expr {
public:
    Expr(int line, int column) : line(line), column(column) {}
    virtual ~Expr() = default;

    int line;
    int column;
};

class LiteralExpr : public Expr {
public:
    LiteralExpr(int value, int line, int column)
        : Expr(line, column), value(value) {}

    int value;
};

class VariableExpr : public Expr  {
    public:
    VariableExpr(Token name, int line, int column)
        : Expr(line, column), name(std::move(name)) {}

    Token name;
};

class UnaryExpr : public Expr {
public:
    UnaryExpr(OperatorType op, std::unique_ptr<Expr> right, int line, int column)
        : Expr(line, column), op(op), right(std::move(right)) {}

    OperatorType op;
    std::unique_ptr<Expr> right;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(std::unique_ptr<Expr> left, OperatorType op, std::unique_ptr<Expr> right, int line, int column)
        : Expr(line, column), left(std::move(left)), op(op), right(std::move(right)) {}

    std::unique_ptr<Expr> left;
    OperatorType op;
    std::unique_ptr<Expr> right;
};

class AssignmentExpr : public Expr {
public:
    AssignmentExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> value, int line, int column)
        : Expr(line, column),
          left(std::move(left)),
          value(std::move(value)) {}

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> value;
};

class CallExpr : public Expr {
public:
    CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> arguments, int line, int column)
        : Expr(line, column),
          callee(std::move(callee)),
          arguments(std::move(arguments)) {}

    std::unique_ptr<Expr> callee;
    std::vector<std::unique_ptr<Expr>> arguments;
};