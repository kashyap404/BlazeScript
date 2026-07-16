#include "scanner.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and", TokenType::AND},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"fn", TokenType::FN},
    {"for", TokenType::FOR},
    {"if", TokenType::IF},
    {"let", TokenType::LET},
    {"const", TokenType::CONST},
    {"or", TokenType::OR},
    {"return", TokenType::RETURN},
    {"true", TokenType::TRUE},
    {"while", TokenType::WHILE},
    {"null", TokenType::NIL},
    {"i32", TokenType::I32},
    {"i64", TokenType::I64},
    {"u32", TokenType::U32},
    {"u64", TokenType::U64},
    {"f32", TokenType::F32},
    {"f64", TokenType::F64},
    {"bool", TokenType::BOOL},
    {"string", TokenType::STRING_TYPE},
};

Scanner::Scanner(std::string source) : source_(std::move(source)) {}

Scanner Scanner::fromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file: " << path << "\n";
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return Scanner(buffer.str());
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start_ = current_;
        start_column_ = column_;
        scanToken();
    }
    tokens_.emplace_back(TokenType::END_OF_FILE, "", std::monostate{}, line_, column_);
    return tokens_;
}

bool Scanner::isAtEnd() const {
    return current_ >= static_cast<int>(source_.length());
}

char Scanner::advance() {
    column_++;
    return source_[current_++];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    column_++;
    return true;
}

char Scanner::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Scanner::peekNext() const {
    if (current_ + 1 >= static_cast<int>(source_.length())) return '\0';
    return source_[current_ + 1];
}

void Scanner::addToken(TokenType type) {
    addToken(type, std::monostate{});
}

void Scanner::addToken(TokenType type, Literal literal) {
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.emplace_back(type, text, std::move(literal), line_, start_column_);
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case ':': addToken(TokenType::COLON); break;
        case '*': addToken(TokenType::STAR); break;

        case '-':
            addToken(match('>') ? TokenType::ARROW : TokenType::MINUS);
            break;

        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;

        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line_++;
            column_ = 1;
            break;

        case '"': string(); break;

        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                number();
            } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                identifier();
            } else {
                std::cerr << "[line " << line_ << "] Unexpected character: " << c << "\n";
            }
            break;
    }
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line_++;
            column_ = 1;
        }
        advance();
    }

    if (isAtEnd()) {
        std::cerr << "[line " << line_ << "] Unterminated string.\n";
        return;
    }

    advance(); // closing "

    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    addToken(TokenType::STRING, value);
}

void Scanner::number() {
    while (std::isdigit(static_cast<unsigned char>(peek()))) advance();

    if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peekNext()))) {
        advance();
        while (std::isdigit(static_cast<unsigned char>(peek()))) advance();

        double value = std::stod(source_.substr(start_, current_ - start_));
        addToken(TokenType::NUMBER, value);
    } else {
        int value = std::stoi(source_.substr(start_, current_ - start_));
        addToken(TokenType::NUMBER, value);
    }
}

void Scanner::identifier() {
    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') advance();

    std::string text = source_.substr(start_, current_ - start_);
    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
    addToken(type);
}