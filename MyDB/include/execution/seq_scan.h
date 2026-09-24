#pragma once

#include "execution/executor.h"
#include "catalog/table.h"
#include <string>

namespace mydb {

/**
 * SeqScanExecutor executes a sequential scan over a table.
 */
class SeqScanExecutor : public AbstractExecutor {
public:
    SeqScanExecutor(ExecutionContext *exec_ctx, const std::string &table_name);
    ~SeqScanExecutor() override = default;

    void Init() override;
    bool Next(Record *record) override;

private:
    std::string table_name_;
    Table *table_;
    
    // Iterator state (simplified for boiler plate)
    page_id_t current_page_id_;
    uint32_t current_slot_id_;
};

} // namespace mydb
