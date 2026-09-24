#pragma once

#include <cstdint>

namespace mydb {

// Page sizes and IDs
using page_id_t = int32_t;
using frame_id_t = int32_t;

constexpr int PAGE_SIZE = 4096;
constexpr page_id_t INVALID_PAGE_ID = -1;

// Transaction IDs
using txn_id_t = int32_t;
constexpr txn_id_t INVALID_TXN_ID = -1;

// Offset types
using offset_t = uint16_t;

// Other common types...

} // namespace mydb
