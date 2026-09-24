#include "storage/slotted_page.h"
#include <cstring>

namespace mydb {

void SlottedPage::Init(page_id_t page_id, page_id_t prev_page_id) {
    ResetMemory();
    SetPageId(page_id);
    SetPrevPageId(prev_page_id);
    SetNextPageId(INVALID_PAGE_ID);
    SetFreeSpacePointer(PAGE_SIZE);
    SetTupleCount(0);
}

void SlottedPage::SetPageId(page_id_t page_id) {
    memcpy(GetData(), &page_id, sizeof(page_id_t));
}

page_id_t SlottedPage::ReadPageId() const {
    return *reinterpret_cast<const page_id_t*>(GetData());
}

page_id_t SlottedPage::GetPrevPageId() const {
    return *reinterpret_cast<const page_id_t*>(GetData() + 4);
}

void SlottedPage::SetPrevPageId(page_id_t prev_page_id) {
    memcpy(GetData() + 4, &prev_page_id, sizeof(page_id_t));
}

page_id_t SlottedPage::GetNextPageId() const {
    return *reinterpret_cast<const page_id_t*>(GetData() + 8);
}

void SlottedPage::SetNextPageId(page_id_t next_page_id) {
    memcpy(GetData() + 8, &next_page_id, sizeof(page_id_t));
}

uint32_t SlottedPage::GetFreeSpacePointer() const {
    return *reinterpret_cast<const uint32_t*>(GetData() + 12);
}

void SlottedPage::SetFreeSpacePointer(uint32_t offset) {
    memcpy(GetData() + 12, &offset, sizeof(uint32_t));
}

uint32_t SlottedPage::GetTupleCount() const {
    return *reinterpret_cast<const uint32_t*>(GetData() + 16);
}

void SlottedPage::SetTupleCount(uint32_t count) {
    memcpy(GetData() + 16, &count, sizeof(uint32_t));
}

uint32_t SlottedPage::GetTupleOffset(uint32_t slot_id) const {
    return *reinterpret_cast<const uint32_t*>(GetData() + HEADER_SIZE + (slot_id * 8));
}

void SlottedPage::SetTupleOffset(uint32_t slot_id, uint32_t offset) {
    memcpy(GetData() + HEADER_SIZE + (slot_id * 8), &offset, sizeof(uint32_t));
}

uint32_t SlottedPage::GetTupleSize(uint32_t slot_id) const {
    return *reinterpret_cast<const uint32_t*>(GetData() + HEADER_SIZE + (slot_id * 8) + 4);
}

void SlottedPage::SetTupleSize(uint32_t slot_id, uint32_t size) {
    memcpy(GetData() + HEADER_SIZE + (slot_id * 8) + 4, &size, sizeof(uint32_t));
}

bool SlottedPage::InsertRecord(const Record &record, RecordId *rid) {
    uint32_t required_space = record.GetSize() + 8; // 8 bytes for slot (offset + size)
    uint32_t free_space_ptr = GetFreeSpacePointer();
    uint32_t tuple_count = GetTupleCount();
    uint32_t space_used = HEADER_SIZE + (tuple_count * 8);

    if (free_space_ptr - space_used < required_space) {
        return false; // Not enough space
    }

    uint32_t new_offset = free_space_ptr - record.GetSize();
    SetFreeSpacePointer(new_offset);
    memcpy(GetData() + new_offset, record.GetData(), record.GetSize());

    SetTupleOffset(tuple_count, new_offset);
    SetTupleSize(tuple_count, record.GetSize());
    
    // Use the member GetPageId() from the base Page class
    rid->page_id_ = GetPageId(); 
    rid->slot_id_ = tuple_count;
    
    SetTupleCount(tuple_count + 1);
    return true;
}

bool SlottedPage::GetRecord(RecordId rid, Record *record) const {
    if (rid.slot_id_ >= GetTupleCount()) return false;
    
    uint32_t size = GetTupleSize(rid.slot_id_);
    if (size == 0) return false; // Tombstone / deleted

    uint32_t offset = GetTupleOffset(rid.slot_id_);
    *record = Record(rid, GetData() + offset, size);
    return true;
}

bool SlottedPage::UpdateRecord(const Record &new_record, RecordId rid) {
    if (rid.slot_id_ >= GetTupleCount()) return false;
    
    uint32_t old_size = GetTupleSize(rid.slot_id_);
    if (old_size == new_record.GetSize()) {
        // In-place update
        uint32_t offset = GetTupleOffset(rid.slot_id_);
        memcpy(GetData() + offset, new_record.GetData(), old_size);
        return true;
    }
    
    // For a real implementation, you'd handle resizing here (compaction).
    // Let's mark as tombstone and re-insert for simplicity in this boiler plate.
    return false; // Requires full delete + insert handled at a higher level
}

bool SlottedPage::DeleteRecord(RecordId rid) {
    if (rid.slot_id_ >= GetTupleCount()) return false;
    
    // Set size to 0 to indicate a tombstone (deleted record)
    SetTupleSize(rid.slot_id_, 0);
    return true;
}

} // namespace mydb
