#include "storage/disk_manager.h"
#include <iostream>
#include <filesystem>
#include <cstring>
#include <cassert>
#include <stdexcept>

namespace mydb {

DiskManager::DiskManager(const std::string &db_file) : file_name_(db_file) {
    std::string::size_type n = file_name_.rfind('.');
    if (n != std::string::npos) {
        log_io_.open(file_name_.substr(0, n) + ".wal", std::ios::out | std::ios::in | std::ios::binary);
    }
    
    db_io_.open(db_file, std::ios::binary | std::ios::in | std::ios::out);
    
    // if file doesn't exist, create it
    if (!db_io_.is_open()) {
        db_io_.clear();
        // create a new file
        db_io_.open(db_file, std::ios::binary | std::ios::trunc | std::ios::out);
        db_io_.close();
        
        // reopen with read and write
        db_io_.open(db_file, std::ios::binary | std::ios::in | std::ios::out);
        if (!db_io_.is_open()) {
            throw std::runtime_error("Can't open db file");
        }
    }
    
    // Calculate the next page ID based on the file size
    db_io_.seekp(0, std::ios::end);
    std::streampos size = db_io_.tellp();
    next_page_id_ = static_cast<page_id_t>(size / PAGE_SIZE);
}

DiskManager::~DiskManager() {
    ShutDown();
}

void DiskManager::ShutDown() {
    std::scoped_lock<std::mutex> lock(db_io_latch_);
    if (db_io_.is_open()) {
        db_io_.close();
    }
    if (log_io_.is_open()) {
        log_io_.close();
    }
}

void DiskManager::WritePage(page_id_t page_id, const char *page_data) {
    std::scoped_lock<std::mutex> lock(db_io_latch_);
    
    size_t offset = static_cast<size_t>(page_id) * PAGE_SIZE;
    
    // set write cursor to offset
    db_io_.seekp(offset);
    db_io_.write(page_data, PAGE_SIZE);
    
    // check for bad bit
    if (db_io_.bad()) {
        throw std::runtime_error("I/O error while writing");
    }
    
    // needs to flush to keep disk file in sync
    db_io_.flush();
    num_writes_++;
}

void DiskManager::ReadPage(page_id_t page_id, char *page_data) {
    std::scoped_lock<std::mutex> lock(db_io_latch_);
    
    size_t offset = static_cast<size_t>(page_id) * PAGE_SIZE;
    
    // Check if page_id is valid based on file size
    db_io_.seekp(0, std::ios::end);
    if (offset >= static_cast<size_t>(db_io_.tellp())) {
        std::cerr << "Read past end of file" << std::endl;
        return; // Alternatively, throw an exception
    }
    
    // set read cursor to offset
    db_io_.seekg(offset);
    db_io_.read(page_data, PAGE_SIZE);
    
    // if file ends before PAGE_SIZE, fill remaining with 0
    int read_count = db_io_.gcount();
    if (read_count < PAGE_SIZE) {
        memset(page_data + read_count, 0, PAGE_SIZE - read_count);
    }
}

page_id_t DiskManager::AllocatePage() {
    return next_page_id_++;
}

void DiskManager::DeallocatePage(page_id_t page_id) {
    // Basic deallocation is typically a no-op at the disk manager level 
    // unless you implement a free list. For now, it does nothing.
}

} // namespace mydb
