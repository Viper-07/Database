#include "catalog/column.h"

namespace mydb {

Column::Column(const std::string &column_name, TypeId type, uint32_t length)
    : column_name_(column_name), column_type_(type), length_(length) {
    if (type != TypeId::VARCHAR) {
        // Assign default lengths based on type
        switch (type) {
            case TypeId::BOOLEAN: length_ = 1; break;
            case TypeId::TINYINT: length_ = 1; break;
            case TypeId::SMALLINT: length_ = 2; break;
            case TypeId::INTEGER: length_ = 4; break;
            case TypeId::BIGINT: length_ = 8; break;
            case TypeId::TIMESTAMP: length_ = 8; break;
            case TypeId::DECIMAL: length_ = 8; break;
            default: break;
        }
    }
}

} // namespace mydb
