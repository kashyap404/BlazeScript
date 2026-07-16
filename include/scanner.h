#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"

class Scanner {
public:
    explicit Scanner(std::string source);
    std::vector<Token> scanTokens();

private:
    void scanToken();
    void identifier();
    void number();
    void string();

    bool match(char expected);
    char peek() const;
    char peekNext() const;
    char advance();
    bool isAtEnd() const;
    void addToken(TokenType type);
    void addToken(TokenType type, Literal literal);

    static const std::unordered_map<std::string, TokenType> keywords;

    std::string source_;
    std::vector<Token> tokens_;
    int start_ = 0;
    int current_ = 0;
    int line_ = 1;
    int column_ = 1;
    int start_column_ = 1;

};      