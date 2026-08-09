#include "parser.h"
#include <stdexcept>
#include <string>

namespace {
    struct ParseError : public std::runtime_error {
        explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
    };
}

std::unique_ptr<Stmt> Parser::parseStmt() {

    if (match({TokenType::LET}))        return parseVarDeclaration();

    if (match({TokenType::IF}))         return parseIfStmt();
    if (match({TokenType::WHILE}))      return parseWhileStmt();
    if (match({TokenType::RETURN}))     return parseReturnStmt();
    if (match({TokenType::LEFT_BRACE})) return parseBlock();
    return parseExprStmt();
}

std::unique_ptr<Stmt> Parser::parseExprStmt() {
    int line = peek().line_;
    int column = peek().column_;

    std::unique_ptr<Expr> expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");

    return std::make_unique<ExpressionStmt>(std::move(expr), line, column);
}

std::unique_ptr<Stmt> Parser::parseIfStmt() {
    Token ifTok = previous();
    int line = ifTok.line_;
    int column = ifTok.column_;

    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
    std::unique_ptr<Expr> condition = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition.");

    std::unique_ptr<Stmt> thenBranch = parseStmt();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = parseStmt(); 
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch),
      std::move(elseBranch), line, column);
}

std::unique_ptr<Stmt> Parser::parseWhileStmt() {
    Token whileTok = previous();
    int line = whileTok.line_;
    int column = whileTok.column_;

    consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
    std::unique_ptr<Expr> condition = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after while condition.");

    std::unique_ptr<Stmt> body = parseStmt();

    return std::make_unique<WhileStmt>(std::move(condition), std::move(body), line, column);
}

std::unique_ptr<Stmt> Parser::parseReturnStmt() {
    Token returnTok = previous();
    int line = returnTok.line_;
    int column = returnTok.column_;

    std::unique_ptr<Expr> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = parseExpression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");

    return std::make_unique<ReturnStmt>(std::move(value), line, column);
}

std::unique_ptr<BlockStmt> Parser::parseBlock() {
    Token braceTok = previous();
    int line = braceTok.line_;
    int column = braceTok.column_;

    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(parseStmt());
    }
    consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");

    return std::make_unique<BlockStmt>(std::move(statements), line, column);
}   
std::unique_ptr<Stmt> Parser::parseVarDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");
    consume(TokenType::COLON, "Expected ':' after variable name.");

    Type* type = parseType();

    std::unique_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = parseExpression(); // Changed to parseExpression() to match your existing functions
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_unique<VarDeclStmt>(name, type, std::move(initializer));
}

Type* Parser::parseType() {
    if (match({TokenType::I32})) return types_.getType(TypeKind::I32);
    if (match({TokenType::I64})) return types_.getType(TypeKind::I64);
    if (match({TokenType::U32})) return types_.getType(TypeKind::U32);
    if (match({TokenType::U64})) return types_.getType(TypeKind::U64);
    if (match({TokenType::F32})) return types_.getType(TypeKind::F32);
    if (match({TokenType::F64})) return types_.getType(TypeKind::F64);
    if (match({TokenType::BOOL})) return types_.getType(TypeKind::BOOL);

    Token bad = peek();
    throw ParseError("[line " + std::to_string(bad.line_) + "] Expected a valid type.");
}