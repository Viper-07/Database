#include "parser/lexer.h"
#include <cctype>
#include <algorithm>

namespace mydb {

Lexer::Lexer(const std::string &sql) : sql_(sql) {}

char Lexer::Peek() const {
    if (pos_ >= sql_.length()) return '\0';
    return sql_[pos_];
}

char Lexer::Advance() {
    if (pos_ >= sql_.length()) return '\0';
    return sql_[pos_++];
}

void Lexer::SkipWhitespace() {
    while (std::isspace(Peek())) {
        Advance();
    }
}

Token Lexer::ReadNumber() {
    std::string num;
    while (std::isdigit(Peek())) {
        num += Advance();
    }
    return Token{TokenType::NUMBER, num};
}

Token Lexer::ReadIdentifierOrKeyword() {
    std::string id;
    while (std::isalnum(Peek()) || Peek() == '_') {
        id += Advance();
    }
    
    std::string upper_id = id;
    std::transform(upper_id.begin(), upper_id.end(), upper_id.begin(), ::toupper);

    if (upper_id == "SELECT") return Token{TokenType::SELECT, id};
    if (upper_id == "FROM") return Token{TokenType::FROM, id};
    if (upper_id == "WHERE") return Token{TokenType::WHERE, id};
    if (upper_id == "INSERT") return Token{TokenType::INSERT, id};
    if (upper_id == "INTO") return Token{TokenType::INTO, id};
    if (upper_id == "VALUES") return Token{TokenType::VALUES, id};
    if (upper_id == "CREATE") return Token{TokenType::CREATE, id};
    if (upper_id == "TABLE") return Token{TokenType::TABLE, id};
    if (upper_id == "INT") return Token{TokenType::INT, id};
    if (upper_id == "VARCHAR") return Token{TokenType::VARCHAR, id};

    return Token{TokenType::IDENTIFIER, id};
}

Token Lexer::ReadString() {
    Advance(); // Skip opening quote
    std::string str;
    while (Peek() != '\'' && Peek() != '\0') {
        str += Advance();
    }
    Advance(); // Skip closing quote
    return Token{TokenType::STRING, str};
}

Token Lexer::NextToken() {
    SkipWhitespace();
    
    if (pos_ >= sql_.length()) {
        return Token{TokenType::EOF_TOKEN, ""};
    }

    char c = Peek();

    if (std::isalpha(c) || c == '_') {
        return ReadIdentifierOrKeyword();
    }
    if (std::isdigit(c)) {
        return ReadNumber();
    }
    if (c == '\'') {
        return ReadString();
    }

    Advance();
    switch (c) {
        case '+': return Token{TokenType::PLUS, "+"};
        case '-': return Token{TokenType::MINUS, "-"};
        case '*': return Token{TokenType::STAR, "*"};
        case '/': return Token{TokenType::SLASH, "/"};
        case '=': return Token{TokenType::EQUALS, "="};
        case '<': return Token{TokenType::LESS, "<"};
        case '>': return Token{TokenType::GREATER, ">"};
        case ',': return Token{TokenType::COMMA, ","};
        case ';': return Token{TokenType::SEMICOLON, ";"};
        case '(': return Token{TokenType::LPAREN, "("};
        case ')': return Token{TokenType::RPAREN, ")"};
        default: return Token{TokenType::INVALID, std::string(1, c)};
    }
}

std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens;
    while (true) {
        Token t = NextToken();
        tokens.push_back(t);
        if (t.type == TokenType::EOF_TOKEN) break;
    }
    return tokens;
}

} // namespace mydb
