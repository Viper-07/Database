#include "parser/parser.h"
#include <iostream>

namespace mydb {

Parser::Parser(const std::string &sql) : lexer_(sql) {
    tokens_ = lexer_.Tokenize();
}

Token Parser::Peek(size_t offset) const {
    if (pos_ + offset >= tokens_.size()) {
        return Token{TokenType::EOF_TOKEN, ""};
    }
    return tokens_[pos_ + offset];
}

Token Parser::Consume() {
    if (pos_ >= tokens_.size()) {
        return Token{TokenType::EOF_TOKEN, ""};
    }
    return tokens_[pos_++];
}

bool Parser::Match(TokenType type) {
    if (Peek().type == type) {
        Consume();
        return true;
    }
    return false;
}

std::unique_ptr<SQLStatement> Parser::Parse() {
    if (Peek().type == TokenType::SELECT) {
        return ParseSelect();
    } else if (Peek().type == TokenType::INSERT) {
        return ParseInsert();
    }
    
    std::cerr << "Syntax Error: Unsupported statement type." << std::endl;
    return nullptr;
}

std::unique_ptr<SelectStatement> Parser::ParseSelect() {
    auto stmt = std::make_unique<SelectStatement>();
    Consume(); // consume SELECT
    
    // Parse select list (e.g., *, id, name)
    if (Match(TokenType::STAR)) {
        stmt->select_list_.push_back("*");
    } else {
        while (Peek().type == TokenType::IDENTIFIER) {
            stmt->select_list_.push_back(Consume().value);
            if (!Match(TokenType::COMMA)) break;
        }
    }
    
    if (!Match(TokenType::FROM)) {
        std::cerr << "Syntax Error: Expected FROM clause." << std::endl;
        return nullptr;
    }
    
    if (Peek().type != TokenType::IDENTIFIER) {
        std::cerr << "Syntax Error: Expected table name." << std::endl;
        return nullptr;
    }
    stmt->table_name_ = Consume().value;
    
    Match(TokenType::SEMICOLON); // Optional semicolon
    
    return stmt;
}

std::unique_ptr<InsertStatement> Parser::ParseInsert() {
    auto stmt = std::make_unique<InsertStatement>();
    Consume(); // consume INSERT
    
    if (!Match(TokenType::INTO)) {
        std::cerr << "Syntax Error: Expected INTO." << std::endl;
        return nullptr;
    }
    
    if (Peek().type != TokenType::IDENTIFIER) {
        std::cerr << "Syntax Error: Expected table name." << std::endl;
        return nullptr;
    }
    stmt->table_name_ = Consume().value;
    
    if (!Match(TokenType::VALUES)) {
        std::cerr << "Syntax Error: Expected VALUES." << std::endl;
        return nullptr;
    }
    
    if (!Match(TokenType::LPAREN)) {
        std::cerr << "Syntax Error: Expected '('." << std::endl;
        return nullptr;
    }
    
    while (Peek().type == TokenType::NUMBER || Peek().type == TokenType::STRING) {
        stmt->values_.push_back(Consume().value);
        if (!Match(TokenType::COMMA)) break;
    }
    
    if (!Match(TokenType::RPAREN)) {
        std::cerr << "Syntax Error: Expected ')'." << std::endl;
        return nullptr;
    }
    
    Match(TokenType::SEMICOLON); // Optional semicolon
    
    return stmt;
}

} // namespace mydb
