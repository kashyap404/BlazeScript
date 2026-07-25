#include "parser.h"
#include <stdexcept>
#include <string>

namespace {
    struct ParseError : public std::runtime_error {
        explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
    };
}

OperatorType tokenTypeToOperator(TokenType type) {
    switch (type) {
        case TokenType::PLUS:          return OperatorType::PLUS;
        case TokenType::MINUS:         return OperatorType::MINUS;
        case TokenType::STAR:          return OperatorType::STAR;
        case TokenType::SLASH:         return OperatorType::SLASH;
        case TokenType::BANG:          return OperatorType::BANG;
        case TokenType::AND:           return OperatorType::AND;
        case TokenType::OR:            return OperatorType::OR;
        case TokenType::EQUAL_EQUAL:   return OperatorType::EQUAL_EQUAL;
        case TokenType::BANG_EQUAL:    return OperatorType::BANG_EQUAL;
        case TokenType::GREATER:       return OperatorType::GREATER;
        case TokenType::GREATER_EQUAL: return OperatorType::GREATER_EQUAL;
        case TokenType::LESS:          return OperatorType::LESS;
        case TokenType::LESS_EQUAL:    return OperatorType::LESS_EQUAL;
        default:
            throw std::invalid_argument("Token type has no corresponding operator");
    }
}

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type_ == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current_++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type_ == TokenType::END_OF_FILE;
}

Token Parser::peek() const {
    return tokens_[current_];
}

Token Parser::previous() const {
    return tokens_[current_ - 1];
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    Token bad = peek();
    throw ParseError("[line " + std::to_string(bad.line_) + ", col " +
                      std::to_string(bad.column_) + "] " + message +
                      " (got '" + bad.lexeme_ + "')");
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::NUMBER})) {
        Token tok = previous();
        int value = std::holds_alternative<int>(tok.literal_)
            ? std::get<int>(tok.literal_)
            : static_cast<int>(std::get<double>(tok.literal_)); 
        return std::make_unique<LiteralExpr>(value, tok.line_, tok.column_);
    }

    if (match({TokenType::IDENTIFIER})) {
        Token tok = previous();
        return std::make_unique<VariableExpr>(tok, tok.line_, tok.column_);
    }

    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return expr; 
    }

    Token bad = peek();
    throw ParseError("[line " + std::to_string(bad.line_) + ", col " +
                      std::to_string(bad.column_) + "] Expected expression, got '" +
                      bad.lexeme_ + "'");
}

std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<UnaryExpr>(tokenTypeToOperator(op.type_), std::move(right), op.line_, op.column_);
    }
    return primary();
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = unary();
    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), tokenTypeToOperator(op.type_), std::move(right), op.line_, op.column_);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), tokenTypeToOperator(op.type_), std::move(right), op.line_, op.column_);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), tokenTypeToOperator(op.type_), std::move(right), op.line_, op.column_);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), tokenTypeToOperator(op.type_), std::move(right), op.line_, op.column_);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseAssignment() {
    std::unique_ptr<Expr> expr = equality();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        std::unique_ptr<Expr> value = parseAssignment();

        if (dynamic_cast<VariableExpr*>(expr.get())) {
            return std::make_unique<AssignmentExpr>(std::move(expr), std::move(value), equals.line_, equals.column_);
        }

        throw ParseError("[line " + std::to_string(equals.line_) + "] Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<Stmt> Parser::parseExpressionStatement() {
    int line = peek().line_;
    int column = peek().column_;
    std::unique_ptr<Expr> expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr), line, column);
}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(parseExpressionStatement());
    }
    return statements;
}