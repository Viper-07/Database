#pragma once

#include "catalog/schema.h"
#include "storage/record.h"
#include "buffer/buffer_pool.h"
#include <string>

namespace mydb {

/**
 * Table manages a collection of Pages (via BufferPoolManager) that contain Records.
 */
class Table {
public:
    Table(BufferPoolManager *bpm, const Schema &schema, const std::string &name);
    ~Table() = default;

    std::string GetName() const { return name_; }
    const Schema &GetSchema() const { return schema_; }
    page_id_t GetFirstPageId() const { return first_page_id_; }

    // Insert a record into the table, returns the RecordId
    bool InsertRecord(const Record &record, RecordId *rid);
    
    // Retrieve a record by its RecordId
    bool GetRecord(RecordId rid, Record *record) const;
    
    // Update a record
    bool UpdateRecord(const Record &record, RecordId rid);
    
    // Delete a record
    bool DeleteRecord(RecordId rid);

private:
    BufferPoolManager *bpm_;
    Schema schema_;
    std::string name_;
    page_id_t first_page_id_{INVALID_PAGE_ID};
};

} // namespace mydb
