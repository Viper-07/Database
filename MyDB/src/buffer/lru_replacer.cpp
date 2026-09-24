#include "buffer/lru_replacer.h"

namespace mydb {

LRUReplacer::LRUReplacer(size_t num_pages) : capacity_(num_pages) {}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
    std::scoped_lock<std::mutex> lock(latch_);
    if (lru_list_.empty()) {
        return false;
    }
    // LRU is at the back of the list
    *frame_id = lru_list_.back();
    lru_map_.erase(*frame_id);
    lru_list_.pop_back();
    return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = lru_map_.find(frame_id);
    if (it != lru_map_.end()) {
        lru_list_.erase(it->second);
        lru_map_.erase(it);
    }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = lru_map_.find(frame_id);
    if (it == lru_map_.end()) {
        // Most recently used is placed at the front
        lru_list_.push_front(frame_id);
        lru_map_[frame_id] = lru_list_.begin();
    }
}

size_t LRUReplacer::Size() {
    std::scoped_lock<std::mutex> lock(latch_);
    return lru_list_.size();
}

} // namespace mydb
