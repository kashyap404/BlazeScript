#pragma once
#include <string>
#include <variant>
#include "token_types.h"
#include <utility>

using Literal = std::variant<std::monostate, int, double, std::string>;

class Token {
public:
    Token(TokenType type, std::string lexeme, Literal literal, int line, int column)
        : type_(type), lexeme_(std::move(lexeme)),
          literal_(std::move(literal)), line_(line), column_(column) {}

    std::string toString() const;


private:
    TokenType type_;
    std::string lexeme_;
    Literal literal_;
    int line_;
    int column_;
};