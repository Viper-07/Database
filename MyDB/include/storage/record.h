#pragma once

#include "common/types.h"
#include <vector>
#include <string>

namespace mydb {

using slot_id_t = int32_t;

struct RecordId {
    page_id_t page_id_{INVALID_PAGE_ID};
    slot_id_t slot_id_{0};

    bool operator==(const RecordId &other) const {
        return page_id_ == other.page_id_ && slot_id_ == other.slot_id_;
    }
};

/**
 * Record represents a single row in a table. For now, it simply holds raw bytes.
 * Later, this will be integrated with the Schema/Column classes to interpret fields.
 */
class Record {
public:
    Record() = default;
    
    // Construct a record from raw bytes
    Record(RecordId rid, const char *data, size_t size);
    
    // Construct a record without an ID yet
    Record(const char *data, size_t size);

    ~Record() = default;

    inline const char *GetData() const { return data_.data(); }
    inline size_t GetSize() const { return data_.size(); }
    inline RecordId GetRecordId() const { return rid_; }
    inline void SetRecordId(RecordId rid) { rid_ = rid; }

private:
    RecordId rid_;
    std::vector<char> data_;
};

} // namespace mydb
