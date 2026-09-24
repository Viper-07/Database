#include "execution/seq_scan.h"
#include "storage/slotted_page.h"
#include <iostream>

namespace mydb {

SeqScanExecutor::SeqScanExecutor(ExecutionContext *exec_ctx, const std::string &table_name)
    : AbstractExecutor(exec_ctx), table_name_(table_name) {
}

void SeqScanExecutor::Init() {
    table_ = exec_ctx_->GetCatalog()->GetTable(table_name_);
    if (table_ != nullptr) {
        current_page_id_ = table_->GetFirstPageId();
        current_slot_id_ = 0;
    } else {
        current_page_id_ = INVALID_PAGE_ID;
        std::cerr << "Table not found for SeqScan: " << table_name_ << std::endl;
    }
}

bool SeqScanExecutor::Next(Record *record) {
    if (table_ == nullptr || current_page_id_ == INVALID_PAGE_ID) {
        return false;
    }

    BufferPoolManager *bpm = exec_ctx_->GetBPM();
    
    while (current_page_id_ != INVALID_PAGE_ID) {
        Page *page = bpm->FetchPage(current_page_id_);
        if (page == nullptr) return false;

        auto *slotted_page = reinterpret_cast<SlottedPage *>(page);
        
        // Scan the current page
        while (slotted_page->GetRecord({current_page_id_, static_cast<slot_id_t>(current_slot_id_)}, record)) {
            current_slot_id_++;
            bpm->UnpinPage(current_page_id_, false);
            return true; // Found a valid record
        }

        // Move to next page
        page_id_t next_page_id = slotted_page->GetNextPageId();
        bpm->UnpinPage(current_page_id_, false);
        
        current_page_id_ = next_page_id;
        current_slot_id_ = 0;
    }

    return false; // EOF
}

} // namespace mydb
