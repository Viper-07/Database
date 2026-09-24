#pragma once

#include "parser/token.h"
#include <string>
#include <vector>

namespace mydb {

/**
 * Lexer converts a raw SQL string into a stream of tokens.
 */
class Lexer {
public:
    explicit Lexer(const std::string &sql);

    // Returns the next token from the input stream
    Token NextToken();
    
    // Tokenizes the entire string
    std::vector<Token> Tokenize();

private:
    std::string sql_;
    size_t pos_{0};

    char Peek() const;
    char Advance();
    void SkipWhitespace();
    
    Token ReadNumber();
    Token ReadIdentifierOrKeyword();
    Token ReadString();
};

} // namespace mydb
