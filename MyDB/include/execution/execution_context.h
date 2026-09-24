#pragma once

#include "catalog/catalog.h"
#include "buffer/buffer_pool.h"

namespace mydb {

/**
 * ExecutionContext stores all the context necessary to run an executor,
 * like pointers to the catalog, buffer pool, and current transaction context.
 */
class ExecutionContext {
public:
    ExecutionContext(Catalog *catalog, BufferPoolManager *bpm)
        : catalog_(catalog), bpm_(bpm) {}
    
    Catalog *GetCatalog() { return catalog_; }
    BufferPoolManager *GetBPM() { return bpm_; }

private:
    Catalog *catalog_;
    BufferPoolManager *bpm_;
};

} // namespace mydb
