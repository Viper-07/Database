#include "catalog/catalog.h"

namespace mydb {

Catalog::Catalog(BufferPoolManager *bpm) : bpm_(bpm) {}

Table* Catalog::CreateTable(const std::string &table_name, const Schema &schema) {
    if (tables_.find(table_name) != tables_.end()) {
        return nullptr; // Table already exists
    }

    auto table = std::make_unique<Table>(bpm_, schema, table_name);
    auto *table_ptr = table.get();
    tables_.emplace(table_name, std::move(table));
    return table_ptr;
}

Table* Catalog::GetTable(const std::string &table_name) {
    auto it = tables_.find(table_name);
    if (it != tables_.end()) {
        return it->second.get();
    }
    return nullptr;
}

} // namespace mydb
