#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "parse.h"

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {

}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {

}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {

}

int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {

}	

int validate_db_header(int fd, struct dbheader_t **headerOut) {

}

int create_db_header(int fd, struct dbheader_t **headerOut) {
    // Allocate memory for the database header
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == -1){
        return handle_file_error("Error allocating memory for database header");
    }

    // Initialize the header fields
    header->version = 0x1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);

    // Write the header to the file
    *headerOut = header;
    return STATUS_SUCCESS;
}


