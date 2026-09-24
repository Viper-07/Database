# MyDB

A lightweight Relational Database Management System built from scratch in C++17. Features a custom storage engine, LRU buffer pool, slotted-page records, a schema catalog, a custom SQL parser, and a Volcano-style execution engine. Designed to explore the end-to-end internal architecture of modern relational databases.

## 🏗️ Architecture

MyDB is built with a modular architecture reflecting modern DBMS design:

- **Storage Engine**: Manages disk I/O, reading and writing 4KB blocks to a `.db` file using a `DiskManager`. Implements a Slotted-Page format to store variable-length database records efficiently without fragmentation.
- **Buffer Pool Manager**: An LRU (Least Recently Used) cache manager that brings pages from disk into memory, serving them to higher-level operators and minimizing disk I/O.
- **Catalog**: An in-memory schema manager tracking tables, columns, and their memory layouts (data types and tuple offsets).
- **SQL Parser**: A custom Lexer and recursive-descent Parser that tokenizes raw SQL strings and builds an Abstract Syntax Tree (AST).
- **Execution Engine**: A Volcano-style iterator model containing operators like `SeqScanExecutor` and `InsertExecutor` to pull tuples from the storage engine.

## 🚀 Getting Started

### Prerequisites
- CMake (3.14+)
- A C++17 compatible compiler (GCC, Clang, or MSVC)

### Build Instructions

```bash
# 1. Navigate to the project folder
cd MyDB

# 2. Create a build directory
mkdir build
cd build

# 3. Configure and compile
cmake ..
cmake --build .
```

### Running the REPL
After compiling, you can launch the interactive database shell:
```bash
# On Windows
.\Debug\mydb.exe  # Or .\mydb.exe depending on compiler

# On Linux/macOS
./mydb
```

You can then run basic SQL statements through the execution engine:
```sql
mydb> INSERT INTO users VALUES (1, 'Alice');
mydb> SELECT * FROM users;
```

## 📄 License
This project is open-source and available under the MIT License.
