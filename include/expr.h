#pragma once
#include <memory>
#include <utility>
#include "token.h"
enum class OperatorType {
    PLUS,
    MINUS,
    STAR,
    SLASH,
    BANG,AND,OR,
    BANG_EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL
};
OperatorType tokenTypeToOperator(TokenType type);
class Expr {
public:
    virtual ~Expr() = default;
};
class LiteralExpr : public Expr {
public:
    explicit LiteralExpr(Literal value)
        : value(std::move(value)) {}
    Literal value;
};
class VariableExpr : public Expr {
public:
    explicit VariableExpr(Token name)
        : name(std::move(name)) {}
    Token name;
};
class UnaryExpr : public Expr {
public:
    UnaryExpr(OperatorType op, std::unique_ptr<Expr> right)
        : op(op),
          right(std::move(right)) {}
    OperatorType op;
    std::unique_ptr<Expr> right;
};
class BinaryExpr : public Expr {
public:
    BinaryExpr(std::unique_ptr<Expr> left, OperatorType op, std::unique_ptr<Expr> right)
        : left(std::move(left)),
          op(op),
          right(std::move(right)) {}
    std::unique_ptr<Expr> left;
    OperatorType op;
    std::unique_ptr<Expr> right;
};
class AssignmentExpr : public Expr {
public:
    AssignmentExpr(Token name, std::unique_ptr<Expr> value)
        : name(std::move(name)),
          value(std::move(value)) {}

    Token name;
    std::unique_ptr<Expr> value;
};