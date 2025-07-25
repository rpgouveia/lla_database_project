#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"
#include "common.h"
#include "validation.h"

int handle_file_error(const char *error_message) {
    perror(error_message);
    return STATUS_ERROR;
}

int file_exists(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd != -1) {
        close(fd);
        printf("File %s exists.\n", filename);
        return STATUS_ERROR;
    }
}

