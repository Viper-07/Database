#include "catalog/schema.h"

namespace mydb {

Schema::Schema(const std::vector<Column> &columns) : columns_(columns) {
    uint32_t curr_offset = 0;
    for (auto &col : columns_) {
        col.SetOffset(curr_offset);
        curr_offset += col.GetLength();
    }
    length_ = curr_offset;
}

int32_t Schema::GetColIdx(const std::string &col_name) const {
    for (uint32_t i = 0; i < columns_.size(); ++i) {
        if (columns_[i].GetName() == col_name) {
            return i;
        }
    }
    return -1;
}

} // namespace mydb
