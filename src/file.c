#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"
#include "common.h"
#include "validation.h"


int create_db_file(char *filename) {
    // Check if the file already exists
    int db_file = file_exists(filename);
    
    // If the file does not exist, create it
    db_file = open(filename, O_RDWR | O_CREAT, 0644);
    if (db_file == -1) {
        return handle_file_error("Error creating database file");
    }
    return db_file;
}

int open_db_file(char *filename) {
    int db_file = open(filename, O_RDWR, 0644);
    if (db_file == -1) {
        return handle_file_error("Error opening database file");
    }
    return db_file;
}


