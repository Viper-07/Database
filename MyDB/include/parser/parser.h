#pragma once

#include "parser/lexer.h"
#include "parser/ast.h"
#include <memory>
#include <vector>

namespace mydb {

/**
 * Parses a stream of tokens into an Abstract Syntax Tree (AST).
 * Note: A real DBMS would often use Bison/Yacc/ANTLR for this. This is a simple recursive descent parser.
 */
class Parser {
public:
    explicit Parser(const std::string &sql);

    std::unique_ptr<SQLStatement> Parse();

private:
    Lexer lexer_;
    std::vector<Token> tokens_;
    size_t pos_{0};

    Token Peek(size_t offset = 0) const;
    Token Consume();
    bool Match(TokenType type);

    std::unique_ptr<SelectStatement> ParseSelect();
    std::unique_ptr<InsertStatement> ParseInsert();
};

} // namespace mydb
