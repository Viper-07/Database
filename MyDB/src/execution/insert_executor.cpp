#include "execution/insert_executor.h"
#include <iostream>
#include <sstream>

namespace mydb {

InsertExecutor::InsertExecutor(ExecutionContext *exec_ctx, InsertStatement *plan)
    : AbstractExecutor(exec_ctx), plan_(plan) {
}

void InsertExecutor::Init() {
    has_run_ = false;
}

bool InsertExecutor::Next(Record *record) {
    if (has_run_) {
        return false; // Only insert once
    }

    Table *table = exec_ctx_->GetCatalog()->GetTable(plan_->table_name_);
    if (table == nullptr) {
        std::cerr << "Table not found: " << plan_->table_name_ << std::endl;
        return false;
    }

    // Very naive serialization: Just concatenate values with commas as a raw string
    std::stringstream ss;
    for (size_t i = 0; i < plan_->values_.size(); ++i) {
        ss << plan_->values_[i];
        if (i < plan_->values_.size() - 1) {
            ss << ",";
        }
    }
    std::string raw_data = ss.str();
    
    Record new_record(raw_data.c_str(), raw_data.length());
    RecordId rid;
    
    bool success = table->InsertRecord(new_record, &rid);
    if (success) {
        // Return a dummy record indicating rows modified
        *record = Record("1 row inserted", 14);
    }
    
    has_run_ = true;
    return success;
}

} // namespace mydb
