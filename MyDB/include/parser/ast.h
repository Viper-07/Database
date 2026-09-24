#pragma once

#include <string>
#include <vector>
#include <memory>

namespace mydb {

enum class StatementType {
    INVALID,
    SELECT,
    INSERT,
    CREATE_TABLE
};

class SQLStatement {
public:
    virtual ~SQLStatement() = default;
    virtual StatementType GetType() const { return StatementType::INVALID; }
};

class SelectStatement : public SQLStatement {
public:
    StatementType GetType() const override { return StatementType::SELECT; }
    
    std::vector<std::string> select_list_;
    std::string table_name_;
    // Optional WHERE clause structure would go here
};

class InsertStatement : public SQLStatement {
public:
    StatementType GetType() const override { return StatementType::INSERT; }
    
    std::string table_name_;
    std::vector<std::string> values_;
};

} // namespace mydb
