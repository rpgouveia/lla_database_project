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

void list_employees(struct dbheader_t *db_header, struct employee_t *employees) {

}

int add_employee(struct dbheader_t *db_header, struct employee_t *employees, char *addstring) {

}

int read_employees(int fd, struct dbheader_t *db_header, struct employee_t **employeesOut) {

}

void output_file(int fd, struct dbheader_t *db_header, struct employee_t *employees) {
    // Validate the file descriptor
    if (fd < 0) {
        return handle_file_error("Invalid file descriptor");
    }

    // Pack the header fields from "host byte order" to "network byte order" (from memory to disc)
    db_header->version = htons(db_header->version);
    db_header->count = htons(db_header->count);
    db_header->magic = htonl(db_header->magic);
    db_header->filesize = htonl(db_header->filesize);

    // Write the header to the file (for now)
    lseek(fd, 0, SEEK_SET);
    write(fd, db_header, sizeof(struct dbheader_t));
    
    
    
}	

int validate_db_header(int fd, struct dbheader_t **headerOut) {
    // Validate the file descriptor
    if (fd < 0) {
        return handle_file_error("Invalid file descriptor");
    }

    // Allocate memory for the database header
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == -1) {
        return handle_file_error("Error allocating memory for database header");
    }

    // [Good practice] Be careful with Endianess when reading the header (big-endian vs little-endian)
    ssize_t bytes_read = read(fd, header, sizeof(struct dbheader_t));
    if (bytes_read != sizeof(struct dbheader_t)) {
        perror("Error reading database header");
        free(header);
        return STATUS_ERROR;
    }

    // Unpack the header fields from "network byte order" to "host byte order" (from disc to memory)
    // https://www.gta.ufrj.br/ensino/eel878/sockets/htonsman.html
    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);

    // Validate the header fields
    if (header->version != 0x1) {
        fprintf(stderr, "Unsupported database version: %u\n", header->version);
        free(header);
        return STATUS_ERROR;
    }
    if (header->magic != HEADER_MAGIC) {
        fprintf(stderr, "Invalid header number: 0x%x\n", header->magic);
        free(header);
        return STATUS_ERROR;
    }
    struct stat db_stat = {0};
    fstat(fd, &db_stat);
    if (header->filesize != db_stat.st_size) {
        printf("Corrupted database file detected.\n");
        free(header);
        return STATUS_ERROR;
    }

    // Set the output header pointer
    *headerOut = header;
    return STATUS_SUCCESS;
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


