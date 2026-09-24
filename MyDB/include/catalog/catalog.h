#pragma once

#include "catalog/table.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace mydb {

class Catalog {
public:
    explicit Catalog(BufferPoolManager *bpm);
    ~Catalog() = default;

    // Create a new table
    Table* CreateTable(const std::string &table_name, const Schema &schema);

    // Get an existing table
    Table* GetTable(const std::string &table_name);

private:
    BufferPoolManager *bpm_;
    std::unordered_map<std::string, std::unique_ptr<Table>> tables_;
};

} // namespace mydb
