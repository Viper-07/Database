#include "buffer/buffer_pool.h"
#include <cassert>

namespace mydb {

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager, Replacer *replacer)
    : pool_size_(pool_size), disk_manager_(disk_manager), replacer_(replacer) {
    pages_ = new Page[pool_size_];
    for (size_t i = 0; i < pool_size_; ++i) {
        free_list_.push_back(static_cast<frame_id_t>(i));
    }
}

BufferPoolManager::~BufferPoolManager() {
    FlushAllPages();
    delete[] pages_;
}

Page *BufferPoolManager::FetchPage(page_id_t page_id) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = page_table_.find(page_id);
    if (it != page_table_.end()) {
        frame_id_t frame_id = it->second;
        Page *page = &pages_[frame_id];
        page->pin_count_++;
        replacer_->Pin(frame_id);
        return page;
    }

    // Need to bring page from disk
    frame_id_t frame_id = -1;
    if (!free_list_.empty()) {
        frame_id = free_list_.front();
        free_list_.pop_front();
    } else if (replacer_->Victim(&frame_id)) {
        Page *victim = &pages_[frame_id];
        if (victim->is_dirty_) {
            disk_manager_->WritePage(victim->GetPageId(), victim->GetData());
        }
        page_table_.erase(victim->GetPageId());
    } else {
        return nullptr; // No free frames and all frames pinned
    }

    Page *page = &pages_[frame_id];
    disk_manager_->ReadPage(page_id, page->GetData());
    page->page_id_ = page_id;
    page->pin_count_ = 1;
    page->is_dirty_ = false;
    
    page_table_[page_id] = frame_id;
    replacer_->Pin(frame_id);

    return page;
}

bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = page_table_.find(page_id);
    if (it == page_table_.end()) {
        return false;
    }

    frame_id_t frame_id = it->second;
    Page *page = &pages_[frame_id];
    
    if (page->pin_count_ <= 0) {
        return false;
    }

    page->pin_count_--;
    if (is_dirty) {
        page->is_dirty_ = true;
    }

    if (page->pin_count_ == 0) {
        replacer_->Unpin(frame_id);
    }
    
    return true;
}

bool BufferPoolManager::FlushPage(page_id_t page_id) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = page_table_.find(page_id);
    if (it == page_table_.end() || page_id == INVALID_PAGE_ID) {
        return false;
    }

    frame_id_t frame_id = it->second;
    Page *page = &pages_[frame_id];
    disk_manager_->WritePage(page->GetPageId(), page->GetData());
    page->is_dirty_ = false;
    return true;
}

Page *BufferPoolManager::NewPage(page_id_t *page_id) {
    std::scoped_lock<std::mutex> lock(latch_);
    frame_id_t frame_id = -1;
    if (!free_list_.empty()) {
        frame_id = free_list_.front();
        free_list_.pop_front();
    } else if (replacer_->Victim(&frame_id)) {
        Page *victim = &pages_[frame_id];
        if (victim->is_dirty_) {
            disk_manager_->WritePage(victim->GetPageId(), victim->GetData());
        }
        page_table_.erase(victim->GetPageId());
    } else {
        return nullptr;
    }

    *page_id = disk_manager_->AllocatePage();
    Page *page = &pages_[frame_id];
    page->page_id_ = *page_id;
    page->pin_count_ = 1;
    page->is_dirty_ = false;
    page->ResetMemory();

    page_table_[*page_id] = frame_id;
    replacer_->Pin(frame_id);

    return page;
}

bool BufferPoolManager::DeletePage(page_id_t page_id) {
    std::scoped_lock<std::mutex> lock(latch_);
    auto it = page_table_.find(page_id);
    if (it == page_table_.end()) {
        return true; // doesn't exist
    }

    frame_id_t frame_id = it->second;
    Page *page = &pages_[frame_id];
    
    if (page->pin_count_ > 0) {
        return false; // pinned, can't delete
    }

    page_table_.erase(page_id);
    replacer_->Pin(frame_id); // remove from replacer since it will be free
    page->page_id_ = INVALID_PAGE_ID;
    page->is_dirty_ = false;
    page->ResetMemory();
    free_list_.push_back(frame_id);
    
    disk_manager_->DeallocatePage(page_id);
    return true;
}

void BufferPoolManager::FlushAllPages() {
    std::scoped_lock<std::mutex> lock(latch_);
    for (auto const& [page_id, frame_id] : page_table_) {
        Page *page = &pages_[frame_id];
        if (page->is_dirty_) {
            disk_manager_->WritePage(page->GetPageId(), page->GetData());
            page->is_dirty_ = false;
        }
    }
}

} // namespace mydb
