#pragma once

#include "index/index.h"
#include "buffer/buffer_pool.h"

namespace mydb {

/**
 * B+Tree index implementation backed by the BufferPoolManager.
 */
class BTreeIndex : public Index {
public:
    // Requires buffer pool manager to allocate/fetch B+Tree nodes
    explicit BTreeIndex(BufferPoolManager *bpm, page_id_t root_page_id = INVALID_PAGE_ID);
    ~BTreeIndex() override = default;

    void InsertEntry(const Record &key, RecordId rid) override;
    void DeleteEntry(const Record &key, RecordId rid) override;
    void ScanKey(const Record &key, std::vector<RecordId> *result) override;

private:
    BufferPoolManager *bpm_;
    page_id_t root_page_id_;
    
    // Internal helper methods for recursive tree traversal will go here
};

} // namespace mydb
