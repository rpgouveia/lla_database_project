#ifndef FILE_H
#define FILE_H

/**
 * @brief Creates a database file or opens existing one
 * 
 * Creates a new database file with read/write permissions (0644) if it doesn't exist,
 * or opens the existing file. The file is opened in read/write mode.
 * 
 * @param filename Path to the database file to create/open
 * @return File descriptor (>= 0) on success, STATUS_ERROR on failure
 */
int create_db_file(char *filename);

/**
 * @brief Opens an existing database file
 * 
 * Opens an existing database file in read/write mode. If the file doesn't exist,
 * the function will fail.
 * 
 * @param filename Path to the database file to open
 * @return File descriptor (>= 0) on success, STATUS_ERROR if file doesn't exist or on error
 */
int open_db_file(char *filename);

#endif
