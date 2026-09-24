#include <iostream>
#include <string>
#include <string_view>
#include "storage/disk_manager.h"
#include "buffer/lru_replacer.h"
#include "buffer/buffer_pool.h"
#include "catalog/catalog.h"
#include "parser/parser.h"
#include "execution/execution_context.h"
#include "execution/seq_scan.h"
#include "execution/insert_executor.h"

using namespace mydb;

void PrintWelcome() {
    std::cout << "==========================================\n";
    std::cout << "             Welcome to MyDB!             \n";
    std::cout << " Type your SQL query (end with ';') or    \n";
    std::cout << " type '\\q' to quit.                      \n";
    std::cout << "==========================================\n";
}

int main() {
    PrintWelcome();

    // 1. Initialize Storage Engine
    DiskManager disk_manager("data/mydb.db");
    LRUReplacer replacer(10); // Buffer pool size 10
    BufferPoolManager bpm(10, &disk_manager, &replacer);

    // 2. Initialize Catalog
    Catalog catalog(&bpm);
    
    // Create a default table for testing
    std::vector<Column> cols = {
        Column("id", TypeId::INTEGER),
        Column("name", TypeId::VARCHAR)
    };
    Schema schema(cols);
    catalog.CreateTable("users", schema);
    
    // 3. Setup Execution Context
    ExecutionContext exec_ctx(&catalog, &bpm);

    std::string query;
    while (true) {
        std::cout << "mydb> ";
        std::getline(std::cin, query);
        
        if (query == "\\q" || query == "exit" || query == "quit") {
            break;
        }
        if (query.empty()) {
            continue;
        }

        // 4. Parse SQL
        Parser parser(query);
        auto ast = parser.Parse();
        
        if (!ast) {
            continue; // Syntax error handled by parser
        }

        // 5. Execute Plan
        if (ast->GetType() == StatementType::INSERT) {
            auto *insert_stmt = static_cast<InsertStatement*>(ast.get());
            InsertExecutor executor(&exec_ctx, insert_stmt);
            
            executor.Init();
            Record record;
            if (executor.Next(&record)) {
                std::cout << std::string_view(record.GetData(), record.GetSize()) << std::endl;
            } else {
                std::cout << "Insert failed." << std::endl;
            }
        } 
        else if (ast->GetType() == StatementType::SELECT) {
            auto *select_stmt = static_cast<SelectStatement*>(ast.get());
            SeqScanExecutor executor(&exec_ctx, select_stmt->table_name_);
            
            executor.Init();
            Record record;
            int count = 0;
            while (executor.Next(&record)) {
                // In a real DB we would project values based on schema here
                // For this skeleton, we just dump the raw bytes representing the row
                std::cout << "[" << count << "] " 
                          << std::string_view(record.GetData(), record.GetSize()) << std::endl;
                count++;
            }
            std::cout << count << " rows returned." << std::endl;
        }
        else {
            std::cout << "Unsupported statement type." << std::endl;
        }
    }

    std::cout << "Shutting down..." << std::endl;
    // Buffer pool destructor flushes all pages to disk
    return 0;
}
