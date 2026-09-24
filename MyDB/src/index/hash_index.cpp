#include "index/hash_index.h"
#include <algorithm>

namespace mydb {

void HashIndex::InsertEntry(const Record &key, RecordId rid) {
    std::scoped_lock<std::mutex> lock(latch_);
    hash_table_[key].push_back(rid);
}

void HashIndex::DeleteEntry(const Record &key, RecordId rid) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = hash_table_.find(key);
    if (it != hash_table_.end()) {
        auto &rids = it->second;
        // Erase the specific RecordId from the vector
        rids.erase(std::remove(rids.begin(), rids.end(), rid), rids.end());
        
        // If the vector is empty, remove the key entirely
        if (rids.empty()) {
            hash_table_.erase(it);
        }
    }
}

void HashIndex::ScanKey(const Record &key, std::vector<RecordId> *result) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = hash_table_.find(key);
    if (it != hash_table_.end()) {
        // Copy the vector of RecordIds into the result
        for (const auto &rid : it->second) {
            result->push_back(rid);
        }
    }
}

} // namespace mydb
