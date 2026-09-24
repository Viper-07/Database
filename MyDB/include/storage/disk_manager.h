#pragma once

#include "common/types.h"
#include <string>
#include <fstream>
#include <mutex>
#include <atomic>

namespace mydb {

/**
 * DiskManager takes care of the allocation and deallocation of pages within a database.
 * It performs the reading and writing of pages to and from disk.
 */
class DiskManager {
public:
    /**
     * Creates a new disk manager that writes to the specified database file.
     * @param db_file the file name of the database file to write to
     */
    explicit DiskManager(const std::string &db_file);

    ~DiskManager();

    /**
     * Shut down the disk manager and close all the file resources.
     */
    void ShutDown();

    /**
     * Write a page to the database file.
     * @param page_id id of the page
     * @param page_data raw page data
     */
    void WritePage(page_id_t page_id, const char *page_data);

    /**
     * Read a page from the database file.
     * @param page_id id of the page
     * @param[out] page_data output buffer
     */
    void ReadPage(page_id_t page_id, char *page_data);

    /**
     * Return the total number of reads/writes for telemetry
     */
    int GetNumFlushes() const { return num_flushes_; }
    int GetNumWrites() const { return num_writes_; }

    /** Allocate a new page id */
    page_id_t AllocatePage();

    /** Deallocate page (not typically implemented for simple disk managers, but good to have) */
    void DeallocatePage(page_id_t page_id);

private:
    // stream to write log file
    std::fstream log_io_;
    
    // file name
    std::string file_name_;
    
    // stream to write db file
    std::fstream db_io_;
    std::mutex db_io_latch_;

    // Telemetry
    std::atomic<int> num_flushes_{0};
    std::atomic<int> num_writes_{0};
    
    // Keep track of the next page ID to allocate
    std::atomic<page_id_t> next_page_id_{0};
};

} // namespace mydb
