#pragma once

#include "common/types.h"
#include <cstring>

namespace mydb {

/**
 * Page is the basic unit of storage within the database system.
 * It provides a wrapper for actual data pages being held in main memory.
 * Size is typically defined by PAGE_SIZE in types.h.
 */
class Page {
public:
    Page() { ResetMemory(); }
    ~Page() = default;

    // Get actual data array
    inline char *GetData() { return data_; }

    // Get page id
    inline page_id_t GetPageId() const { return page_id_; }
    
    // Get pin count (for buffer pool management)
    inline int GetPinCount() const { return pin_count_; }
    
    // Check if the page is dirty
    inline bool IsDirty() const { return is_dirty_; }

protected:
    friend class BufferPoolManager;
    
    inline void ResetMemory() { memset(data_, 0, PAGE_SIZE); }
    
    // The actual data that is stored within a page.
    char data_[PAGE_SIZE]{};
    
    // The ID of this page.
    page_id_t page_id_{INVALID_PAGE_ID};
    
    // The pin count of this page.
    int pin_count_{0};
    
    // True if the page was modified since it was read from disk.
    bool is_dirty_{false};
};

} // namespace mydb
