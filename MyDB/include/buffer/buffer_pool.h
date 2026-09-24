#pragma once

#include "common/types.h"
#include "storage/disk_manager.h"
#include "storage/page.h"
#include "buffer/replacer.h"
#include <mutex>
#include <unordered_map>
#include <list>
#include <memory>

namespace mydb {

class BufferPoolManager {
public:
    BufferPoolManager(size_t pool_size, DiskManager *disk_manager, Replacer *replacer);
    ~BufferPoolManager();

    // Fetch the requested page from the buffer pool.
    Page *FetchPage(page_id_t page_id);

    // Unpin the target page from the buffer pool.
    bool UnpinPage(page_id_t page_id, bool is_dirty);

    // Flush the target page to disk.
    bool FlushPage(page_id_t page_id);

    // Creates a new page in the buffer pool.
    Page *NewPage(page_id_t *page_id);

    // Deletes a page from the buffer pool.
    bool DeletePage(page_id_t page_id);

    // Flushes all the pages in the buffer pool to disk.
    void FlushAllPages();

private:
    size_t pool_size_;
    Page *pages_;
    DiskManager *disk_manager_;
    std::unique_ptr<Replacer> replacer_;
    std::list<frame_id_t> free_list_;
    std::mutex latch_;
    std::unordered_map<page_id_t, frame_id_t> page_table_;
};

} // namespace mydb
