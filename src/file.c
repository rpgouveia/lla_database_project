#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"
#include "common.h"
#include "validation.h"

/**
 * @brief Creates a database file or opens existing one
 * 
 * Implementation details:
 * - Opens file in read/write mode (O_RDWR)
 * - Creates file if it doesn't exist (O_CREAT)
 * - Sets file permissions to 0644 (owner: rw, group: r, others: r)
 * - Uses atomic operation to avoid race conditions
 * - Returns file descriptor for further operations
 */
int create_db_file(char *filename) {
    int db_file = open(filename, O_RDWR | O_CREAT, 0644);
    if (db_file == STATUS_ERROR) {
        return handle_file_error("Error creating database file");
    }
    return db_file;
}

/**
 * @brief Opens an existing database file
 * 
 * Implementation details:
 * - Opens file in read/write mode (O_RDWR)
 * - Does NOT create file if it doesn't exist
 * - Fails if file doesn't exist or cannot be opened
 */
int open_db_file(char *filename) {
    int db_file = open(filename, O_RDWR, 0644);
    if (db_file == STATUS_ERROR) {
        return handle_file_error("Error opening database file");
    }
    return db_file;
}


