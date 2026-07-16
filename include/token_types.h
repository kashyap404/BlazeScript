#pragma once
#include <string>

enum class TokenType {

    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    COLON,          

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    ARROW,
    IDENTIFIER, STRING, NUMBER,
    AND, ELSE, FALSE, FN, FOR, IF, LET,
    OR, RETURN, TRUE, WHILE,CONST,NULL,
    I32, I64,U32,U64,F32,
    F64, BOOL, STRING_TYPE,

    END_OF_FILE
};

std::string tokenTypeToString(TokenType type);