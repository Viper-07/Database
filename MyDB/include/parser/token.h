#pragma once

#include <string>

namespace mydb {

enum class TokenType {
    INVALID,
    EOF_TOKEN,

    // Keywords
    SELECT,
    FROM,
    WHERE,
    INSERT,
    INTO,
    VALUES,
    CREATE,
    TABLE,
    INT,
    VARCHAR,

    // Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUALS,
    NOT_EQUALS,
    GREATER,
    LESS,

    // Punctuation
    COMMA,
    SEMICOLON,
    LPAREN,
    RPAREN,

    // Literals
    IDENTIFIER,
    NUMBER,
    STRING
};

struct Token {
    TokenType type;
    std::string value;

    Token() : type(TokenType::INVALID), value("") {}
    Token(TokenType type, const std::string &value) : type(type), value(value) {}
};

} // namespace mydb
