#pragma once

#include "catalog/column.h"
#include <vector>
#include <memory>

namespace mydb {

class Schema {
public:
    explicit Schema(const std::vector<Column> &columns);

    const std::vector<Column> &GetColumns() const { return columns_; }
    const Column &GetColumn(uint32_t col_idx) const { return columns_[col_idx]; }
    
    uint32_t GetColumnCount() const { return static_cast<uint32_t>(columns_.size()); }
    uint32_t GetLength() const { return length_; }
    
    // Find column index by name. Returns -1 if not found.
    int32_t GetColIdx(const std::string &col_name) const;

private:
    uint32_t length_{0}; // Total length of fixed-size columns
    std::vector<Column> columns_;
};

} // namespace mydb
