#pragma once

#include "index/index.h"
#include <unordered_map>
#include <mutex>
#include <string>
#include <cstring>

namespace mydb {

// A simple hash function for Records (using raw bytes)
struct RecordHasher {
    size_t operator()(const Record &record) const {
        return std::hash<std::string_view>{}(
            std::string_view(record.GetData(), record.GetSize())
        );
    }
};

struct RecordEqual {
    bool operator()(const Record &lhs, const Record &rhs) const {
        if (lhs.GetSize() != rhs.GetSize()) return false;
        return std::memcmp(lhs.GetData(), rhs.GetData(), lhs.GetSize()) == 0;
    }
};

/**
 * HashIndex is a simple in-memory hash table for exact match queries.
 * In a real DBMS, this would be a linear probing or extendible hash table
 * backed by the BufferPoolManager, but this serves as a baseline.
 */
class HashIndex : public Index {
public:
    HashIndex() = default;
    ~HashIndex() override = default;

    void InsertEntry(const Record &key, RecordId rid) override;
    void DeleteEntry(const Record &key, RecordId rid) override;
    void ScanKey(const Record &key, std::vector<RecordId> *result) override;

private:
    std::mutex latch_;
    // Maps a key to a list of RecordIds (since keys might not be unique)
    std::unordered_map<Record, std::vector<RecordId>, RecordHasher, RecordEqual> hash_table_;
};

} // namespace mydb
