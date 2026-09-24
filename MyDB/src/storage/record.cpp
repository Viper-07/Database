#include "storage/record.h"
#include <cstring>

namespace mydb {

Record::Record(RecordId rid, const char *data, size_t size) : rid_(rid) {
    data_.resize(size);
    std::memcpy(data_.data(), data, size);
}

Record::Record(const char *data, size_t size) {
    data_.resize(size);
    std::memcpy(data_.data(), data, size);
}

} // namespace mydb
