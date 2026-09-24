#pragma once

#include "storage/record.h"
#include <vector>

namespace mydb {

/**
 * Abstract class for index implementations (e.g. B+Tree, Hash).
 * It maps a search key (represented as a Record for now, but usually a Tuple) 
 * to a list of RecordIds where the matching tuples are stored in the table.
 */
class Index {
public:
    virtual ~Index() = default;

    /**
     * Insert a key/value pair into the index.
     * @param key the key to insert
     * @param rid the record id to map to
     */
    virtual void InsertEntry(const Record &key, RecordId rid) = 0;

    /**
     * Delete a key/value pair from the index.
     * @param key the key to delete
     * @param rid the record id to delete
     */
    virtual void DeleteEntry(const Record &key, RecordId rid) = 0;

    /**
     * Search the index for a key, returning all matching RecordIds.
     * @param key the key to search for
     * @param[out] result the list of record ids that match the key
     */
    virtual void ScanKey(const Record &key, std::vector<RecordId> *result) = 0;
};

} // namespace mydb
