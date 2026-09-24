# 🗄️ MyDB

<div align="center">

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.14+-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-In%20Development-orange?style=for-the-badge)

**A lightweight Relational Database Management System built from scratch in C++17.**

*Custom storage engine · LRU buffer pool · Slotted pages · Schema catalog · SQL parser · Volcano execution engine*

</div>

---

## 📖 Overview

MyDB is an educational, from-scratch implementation of a relational database system. It is designed to explore the end-to-end internal architecture of modern relational databases — from raw disk I/O all the way up to SQL parsing and query execution.

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────┐
│              SQL Interface (REPL)            │
├─────────────────────────────────────────────┤
│         🔍 SQL Parser  (Lexer + AST)        │
├─────────────────────────────────────────────┤
│     ⚙️  Execution Engine  (Volcano Model)   │
│  SeqScan │ Insert │ Delete │ Update │ Join  │
├─────────────────────────────────────────────┤
│         📋 Catalog  (Tables + Schema)       │
├─────────────────────────────────────────────┤
│     🌳 Index Manager  (B+Tree + Hash)       │
├─────────────────────────────────────────────┤
│     🧠 Buffer Pool Manager  (LRU Policy)    │
├─────────────────────────────────────────────┤
│     💾 Storage Engine  (Slotted Pages)      │
├─────────────────────────────────────────────┤
│         🖴  Disk Manager  (File I/O)        │
└─────────────────────────────────────────────┘
```

| Layer | Component | Description |
|---|---|---|
| 🖴 | **Disk Manager** | Reads/writes raw 4KB page blocks from/to a `.db` file |
| 💾 | **Slotted Page** | Packs variable-length records into pages without fragmentation |
| 🧠 | **Buffer Pool** | LRU in-memory cache to minimize disk I/O |
| 🌳 | **Index Manager** | Hash index (exact match) + B+Tree skeleton (range queries) |
| 📋 | **Catalog** | Tracks tables, schemas, and column types/offsets |
| 🔍 | **SQL Parser** | Custom Lexer + recursive-descent Parser → AST |
| ⚙️ | **Execution Engine** | Volcano iterator model: `SeqScan`, `Insert`, and more |

---

## 📁 Project Structure

```
MyDB/
├── include/          # All header files
│   ├── common/       # Types, constants, config
│   ├── storage/      # DiskManager, Page, SlottedPage, Record
│   ├── buffer/       # BufferPool, LRU/Clock Replacer
│   ├── catalog/      # Catalog, Table, Schema, Column
│   ├── index/        # Index interface, HashIndex, B+Tree
│   ├── parser/       # Token, Lexer, AST, Parser
│   ├── execution/    # Executors (SeqScan, Insert, etc.)
│   ├── transaction/  # Transaction, Lock, IsolationLevel
│   └── wal/          # Log records, WAL writer/reader
│
├── src/              # All implementation files
├── tests/            # Unit & integration tests
├── docs/             # Architecture documentation
├── benchmarks/       # Performance benchmarks
└── examples/         # Usage examples
```

---

## 🚀 Getting Started

### ✅ Prerequisites

- CMake **3.14+**
- A C++17 compatible compiler:
  - **Windows**: Visual Studio 2019+ (with "Desktop development with C++" workload), or MinGW
  - **Linux/macOS**: GCC 9+ or Clang 10+

### 🔨 Build Instructions

```bash
# Clone the repository
git clone https://github.com/Viper-07/Database.git
cd Database/MyDB

# Create build directory
mkdir build && cd build

# Configure (Visual Studio / MSVC)
cmake ..

# Configure (MinGW on Windows)
cmake -G "MinGW Makefiles" ..

# Compile
cmake --build .
```

### ▶️ Running the Interactive SQL Shell

```bash
# Windows (MSVC)
.\Debug\mydb.exe

# Windows (MinGW) or Linux/macOS
./mydb
```

You'll be greeted with an interactive prompt:

```
==========================================
             Welcome to MyDB!
 Type your SQL query (end with ';') or
 type '\q' to quit.
==========================================
mydb> INSERT INTO users VALUES (1, 'Alice');
1 row inserted
mydb> INSERT INTO users VALUES (2, 'Bob');
1 row inserted
mydb> SELECT * FROM users;
[0] 1,Alice
[1] 2,Bob
2 rows returned.
mydb> \q
Shutting down...
```

---

## 🗺️ Roadmap

- [x] Disk Manager + Slotted Page storage
- [x] LRU Buffer Pool Manager
- [x] Schema Catalog (Tables, Columns)
- [x] Hash Index
- [x] SQL Lexer + Parser (SELECT, INSERT)
- [x] SeqScan + Insert Execution Engine
- [ ] B+ Tree Index (full implementation)
- [ ] Write-Ahead Log (WAL) + Crash Recovery
- [ ] Transaction Manager + Lock Manager
- [ ] DELETE / UPDATE executors
- [ ] Query Optimizer (cost-based)
- [ ] Network layer (client/server protocol)

---

## 📄 License

This project is licensed under the **MIT License**. See the [LICENSE](MyDB/LICENSE) file for details.
