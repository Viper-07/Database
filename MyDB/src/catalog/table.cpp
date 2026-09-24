#include "catalog/table.h"
#include "storage/slotted_page.h"
#include <iostream>

namespace mydb {

Table::Table(BufferPoolManager *bpm, const Schema &schema, const std::string &name)
    : bpm_(bpm), schema_(schema), name_(name) {
    // Allocate the first page for this table
    Page *page = bpm_->NewPage(&first_page_id_);
    if (page != nullptr) {
        auto *slotted_page = reinterpret_cast<SlottedPage *>(page);
        slotted_page->Init(first_page_id_);
        bpm_->UnpinPage(first_page_id_, true);
    } else {
        std::cerr << "Failed to allocate first page for table " << name << std::endl;
    }
}

bool Table::InsertRecord(const Record &record, RecordId *rid) {
    if (first_page_id_ == INVALID_PAGE_ID) return false;

    // A real implementation would keep track of free space or iterate through pages.
    // For simplicity, we just try to insert into the first page.
    Page *page = bpm_->FetchPage(first_page_id_);
    if (page == nullptr) return false;

    auto *slotted_page = reinterpret_cast<SlottedPage *>(page);
    bool inserted = slotted_page->InsertRecord(record, rid);
    
    if (inserted) {
        bpm_->UnpinPage(first_page_id_, true); // Dirty
        return true;
    }

    // If full, allocate a new page and append (simplified)
    page_id_t new_page_id;
    Page *new_page = bpm_->NewPage(&new_page_id);
    if (new_page != nullptr) {
        auto *new_slotted_page = reinterpret_cast<SlottedPage *>(new_page);
        new_slotted_page->Init(new_page_id, first_page_id_);
        slotted_page->SetNextPageId(new_page_id);
        
        inserted = new_slotted_page->InsertRecord(record, rid);
        
        bpm_->UnpinPage(new_page_id, true);
        bpm_->UnpinPage(first_page_id_, true);
        
        // Note: For a real system we'd manage a linked list of pages properly
        // instead of just pinning it to the first page.
        return inserted;
    }

    bpm_->UnpinPage(first_page_id_, false);
    return false;
}

bool Table::GetRecord(RecordId rid, Record *record) const {
    Page *page = bpm_->FetchPage(rid.page_id_);
    if (page == nullptr) return false;

    auto *slotted_page = reinterpret_cast<SlottedPage *>(page);
    bool success = slotted_page->GetRecord(rid, record);
    
    bpm_->UnpinPage(rid.page_id_, false);
    return success;
}

bool Table::UpdateRecord(const Record &record, RecordId rid) {
    Page *page = bpm_->FetchPage(rid.page_id_);
    if (page == nullptr) return false;

    auto *slotted_page = reinterpret_cast<SlottedPage *>(page);
    bool success = slotted_page->UpdateRecord(record, rid);
    
    bpm_->UnpinPage(rid.page_id_, success); // Dirty if successful
    return success;
}

bool Table::DeleteRecord(RecordId rid) {
    Page *page = bpm_->FetchPage(rid.page_id_);
    if (page == nullptr) return false;

    auto *slotted_page = reinterpret_cast<SlottedPage *>(page);
    bool success = slotted_page->DeleteRecord(rid);
    
    bpm_->UnpinPage(rid.page_id_, success); // Dirty if successful
    return success;
}

} // namespace mydb
