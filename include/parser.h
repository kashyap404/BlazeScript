#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "expr.h"
#include "stmt.h"
#include "token.h"

#include "function.h"

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
    std::vector<std::unique_ptr<FuncDefn>> functions;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    Program parse();

private:
    // Expressions
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseAssignment();
    std::unique_ptr<Expr> parseOr();
    std::unique_ptr<Expr> parseAnd();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> parseCall();
    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);
    std::unique_ptr<Expr> primary();

    // Statements
    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Stmt> parseExprStmt();
    std::unique_ptr<Stmt> parseIfStmt();
    std::unique_ptr<Stmt> parseElseStmt();
    std::unique_ptr<Stmt> parseWhileStmt();
    std::unique_ptr<Stmt> parseReturnStmt();
    std::unique_ptr<BlockStmt> parseBlock();

    std::unique_ptr<Stmt> parseVarDeclaration();
    Type* parseType();

    // Functions
    std::unique_ptr<FuncDefn> parseFunctionDeclaration();

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    Token advance();
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;
    Token consume(TokenType type, const std::string& message);

    std::vector<Token> tokens_;
    std::size_t current_ = 0;
    TypeTable types_;
};