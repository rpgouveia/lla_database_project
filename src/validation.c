#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"
#include "common.h"
#include "validation.h"

/**
 * @brief Handles file operation errors by printing error message
 * 
 * Implementation details:
 * - Uses perror() to print error message with system error description
 * - Writes to stderr for proper error stream handling
 * - Returns consistent error code for standardized error handling
 * - Does not exit the program, allowing caller to decide next action
 */
int handle_file_error(const char *error_message) {
    perror(error_message);
    return STATUS_ERROR;
}

