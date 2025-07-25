#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database filepath>\n", argv[0]);
    printf("Options:\n");
    printf("\t -n        - Create a new database file\n");
    printf("\t -f <path> - Specify the database file path\n");
    printf("If -n is not specified, the program will attempt to open an existing database file.\n");
}

int main(int argc, char *argv[]) { 
	char *filepath = NULL;
    bool new_file = false;
    int option;
    int database_fd = -1;
    struct dbheader_t *header = NULL;

    while((option = getopt(argc, argv, "nf:")) != -1) {
        switch(option) {
            case 'n':
                new_file = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case '?':
                printf("Unknown option: -%c\n", option);
                break;
            default:
                return STATUS_ERROR;
        }
    }

    if (filepath == NULL) {
        printf("Filepath is a required argument.\n");
        print_usage(argv);
        return STATUS_SUCCESS;
    }

    if (new_file) {
        database_fd = create_db_file(filepath);
        if (database_fd == -1) {
            fprintf(stderr, "Failed to create database file.\n");
            return STATUS_ERROR;
        }

        int create_status = create_db_header(database_fd, &header);
        if (create_status == STATUS_ERROR) {
            fprintf(stderr, "Failed to create database header.\n");
            close(database_fd);
            return STATUS_ERROR;
        }
    } else {
        database_fd = open_db_file(filepath);
        if (database_fd == -1) {
            fprintf(stderr, "Failed to open database file.\n");
            return STATUS_ERROR;
        }
        int validation_status = validate_db_header(database_fd, &header);
        if (validation_status == STATUS_ERROR) {
            fprintf(stderr, "Database header validation failed.\n");
            close(database_fd);
            return STATUS_ERROR;
        }
    }

    printf("New File: %d\n", new_file);
    printf("Filepath: %s\n", filepath);
    output_file(database_fd, header, NULL); // just a test

    return STATUS_SUCCESS;
}
