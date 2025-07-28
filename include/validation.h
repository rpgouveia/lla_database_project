#ifndef VALIDATION_H
#define VALIDATION_H

#include "common.h"

/**
 * @brief Handles file operation errors by printing error message
 * 
 * Prints the provided error message to stderr using perror() and returns
 * a standard error code. This function provides consistent error handling
 * across file operations in the application.
 * 
 * @param error_message Error message to display (should describe the failed operation)
 * @return Always returns STATUS_ERROR to indicate failure
 */
int handle_file_error(const char *error_message);

#endif
