#pragma once

#include <string>
#include <cstdint>

namespace mydb {

enum class TypeId {
    INVALID = 0,
    BOOLEAN,
    TINYINT,
    SMALLINT,
    INTEGER,
    BIGINT,
    DECIMAL,
    VARCHAR,
    TIMESTAMP
};

class Column {
public:
    Column(const std::string &column_name, TypeId type, uint32_t length = 0);

    std::string GetName() const { return column_name_; }
    TypeId GetType() const { return column_type_; }
    uint32_t GetLength() const { return length_; }
    uint32_t GetOffset() const { return column_offset_; }
    
    // Used by Schema to set memory offsets for tuples
    void SetOffset(uint32_t offset) { column_offset_ = offset; }

private:
    std::string column_name_;
    TypeId column_type_;
    uint32_t length_; // For VARCHAR/variable length types
    uint32_t column_offset_{0}; // Byte offset in the tuple
};

} // namespace mydb
