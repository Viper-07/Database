#pragma once

#include "execution/executor.h"
#include "parser/ast.h"

namespace mydb {

/**
 * InsertExecutor executes an insert statement into a table.
 */
class InsertExecutor : public AbstractExecutor {
public:
    InsertExecutor(ExecutionContext *exec_ctx, InsertStatement *plan);
    ~InsertExecutor() override = default;

    void Init() override;
    bool Next(Record *record) override;

private:
    InsertStatement *plan_;
    bool has_run_{false};
};

} // namespace mydb
