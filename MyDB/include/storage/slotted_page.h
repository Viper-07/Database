#pragma once

#include "storage/page.h"
#include "storage/record.h"

namespace mydb {

/**
 * SlottedPage Format:
 * 
 * Header:
 * - Page ID (4 bytes)
 * - Previous Page ID (4 bytes)
 * - Next Page ID (4 bytes)
 * - Free Space Pointer (4 bytes) (Points to end of free space)
 * - Tuple Count (4 bytes)
 * 
 * Slotted Array:
 * - Tuple 1 Offset (4 bytes)
 * - Tuple 1 Size (4 bytes)
 * - Tuple 2 Offset (4 bytes)
 * - ...
 * 
 * Data: (Grows backward from the end of the page)
 * - Tuple N Data
 * - ...
 * - Tuple 2 Data
 * - Tuple 1 Data
 */

class SlottedPage : public Page {
public:
    SlottedPage() = delete;

    void Init(page_id_t page_id, page_id_t prev_page_id = INVALID_PAGE_ID);

    page_id_t GetPrevPageId() const;
    page_id_t GetNextPageId() const;
    void SetPrevPageId(page_id_t prev_page_id);
    void SetNextPageId(page_id_t next_page_id);

    // Inserts a new record into the page. Returns true if successful (enough space).
    bool InsertRecord(const Record &record, RecordId *rid);

    // Updates a record in-place if size matches, or re-inserts.
    bool UpdateRecord(const Record &new_record, RecordId rid);
    
    // Deletes a record from the page (sets size to 0 or tombstone).
    bool DeleteRecord(RecordId rid);

    // Retrieves a record from the page.
    bool GetRecord(RecordId rid, Record *record) const;

private:
    // Helper methods
    void SetPageId(page_id_t page_id);
    page_id_t ReadPageId() const;

    uint32_t GetFreeSpacePointer() const;
    void SetFreeSpacePointer(uint32_t offset);
    
    uint32_t GetTupleCount() const;
    void SetTupleCount(uint32_t count);

    uint32_t GetTupleOffset(uint32_t slot_id) const;
    void SetTupleOffset(uint32_t slot_id, uint32_t offset);
    
    uint32_t GetTupleSize(uint32_t slot_id) const;
    void SetTupleSize(uint32_t slot_id, uint32_t size);

    // Header size (excluding the slotted array)
    static constexpr size_t HEADER_SIZE = 20; 
};

} // namespace mydb
