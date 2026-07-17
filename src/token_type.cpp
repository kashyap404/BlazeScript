#include "token_types.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::MINUS: return "MINUS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::STAR: return "STAR";
        case TokenType::COLON: return "COLON";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::ARROW: return "ARROW";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::AND: return "AND";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::FN: return "FN";
        case TokenType::FOR: return "FOR";
        case TokenType::IF: return "IF";
        case TokenType::LET: return "LET";
        case TokenType::CONST: return "CONST";
        case TokenType::OR: return "OR";
        case TokenType::RETURN: return "RETURN";
        case TokenType::TRUE: return "TRUE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::NIL: return "NIL";
        case TokenType::I32: return "I32";
        case TokenType::I64: return "I64";
        case TokenType::U32: return "U32";
        case TokenType::U64: return "U64";
        case TokenType::F32: return "F32";
        case TokenType::F64: return "F64";
        case TokenType::BOOL: return "BOOL";
        case TokenType::STRING_TYPE: return "STRING_TYPE";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}