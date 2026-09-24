#pragma once

#include "execution/execution_context.h"
#include "storage/record.h"

namespace mydb {

/**
 * Abstract base class for all relational algebra operators (executors).
 * Follows the Volcano iterator model (Init(), Next()).
 */
class AbstractExecutor {
public:
    explicit AbstractExecutor(ExecutionContext *exec_ctx) : exec_ctx_(exec_ctx) {}
    virtual ~AbstractExecutor() = default;

    // Initialize the executor
    virtual void Init() = 0;

    // Yield the next tuple from the executor
    virtual bool Next(Record *record) = 0;

    ExecutionContext *GetExecutorContext() { return exec_ctx_; }

protected:
    ExecutionContext *exec_ctx_;
};

} // namespace mydb
