#include "index/btree.h"
#include <iostream>

namespace mydb {

BTreeIndex::BTreeIndex(BufferPoolManager *bpm, page_id_t root_page_id)
    : bpm_(bpm), root_page_id_(root_page_id) {
}

void BTreeIndex::InsertEntry(const Record &key, RecordId rid) {
    // TODO: Implement B+ Tree insertion
    std::cerr << "BTreeIndex::InsertEntry not yet implemented." << std::endl;
}

void BTreeIndex::DeleteEntry(const Record &key, RecordId rid) {
    // TODO: Implement B+ Tree deletion
    std::cerr << "BTreeIndex::DeleteEntry not yet implemented." << std::endl;
}

void BTreeIndex::ScanKey(const Record &key, std::vector<RecordId> *result) {
    // TODO: Implement B+ Tree search
    std::cerr << "BTreeIndex::ScanKey not yet implemented." << std::endl;
}

} // namespace mydb
