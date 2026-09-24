#pragma once

#include "buffer/replacer.h"
#include <list>
#include <mutex>
#include <unordered_map>

namespace mydb {

class LRUReplacer : public Replacer {
public:
    explicit LRUReplacer(size_t num_pages);
    ~LRUReplacer() override;

    bool Victim(frame_id_t *frame_id) override;
    void Pin(frame_id_t frame_id) override;
    void Unpin(frame_id_t frame_id) override;
    size_t Size() override;

private:
    std::mutex latch_;
    std::list<frame_id_t> lru_list_;
    std::unordered_map<frame_id_t, std::list<frame_id_t>::iterator> lru_map_;
    size_t capacity_;
};

} // namespace mydb
