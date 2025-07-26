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
    // Initialize variables
    char *filepath = NULL;
    char *addstring = NULL;
    // Default values
    bool new_file = false;
    bool list = false;
    int option;
    int database_fd = -1;
    // Initialize pointers for header and employees
    struct dbheader_t *header = NULL;
    struct employee_t *employees = NULL;

    while((option = getopt(argc, argv, "nf:a:l")) != -1) {
        switch(option) {
            case 'n':
                new_file = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'l':
                list = true;
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

    // Read employees from the database file
    int read_status = read_employees(database_fd, header, &employees);
    if (read_status != STATUS_SUCCESS) {
        printf("Failed to read employees from the database file.\n");
        close(database_fd);
        return STATUS_ERROR;
    }

    // Add a new employee
    if (addstring) {
        // Increment the employee count in the header
        header->count++;
        // Reallocate memory for employees array
        employees = realloc(employees, header->count * sizeof(struct employee_t));
        if (employees == NULL) {
            fprintf(stderr, "Failed to reallocate memory for employees array.\n");
            close(database_fd);
            return STATUS_ERROR;
        }
        // Add the new employee
        add_employee(header, employees, addstring);
    }

    if (list) {
        // List all employees
        list_employees(header, employees);
    }

    // Save the output file
    output_file(database_fd, header, employees);

    return STATUS_SUCCESS;
}
