#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database filepath>\n", argv[0]);
    printf("Options:\n");
    printf("\t -n          - Create a new database file\n");
    printf("\t -f <path>   - Specify the database file path\n");
    printf("\t -a <data>   - Add employee (format: name,address,hours)\n");
    printf("\t -l          - List all employees\n");
    printf("\t -r <index>  - Remove employee at specified index (0-based)\n");
    printf("If -n is not specified, the program will attempt to open an existing database file.\n");
}

int main(int argc, char *argv[]) { 
    // Initialize pointers
    char *filepath = NULL;
    char *add_string = NULL;
    char *update_arg = NULL;
    char *update_string = NULL;
    struct dbheader_t *header = NULL;
    struct employee_t *employees = NULL;
    // Initialize variables
    int remove_index = -1;
    int update_index = -1;
    int database_fd = -1;
    int option;
    bool new_file_flag = false;
    bool list_employees_flag = false;
    bool remove_employee_flag = false;
    bool update_employee_flag = false;

    while((option = getopt(argc, argv, "nf:a:lr:u:")) != -1) {
        switch(option) {
            case 'n':
                new_file_flag = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'a':
                add_string = optarg;
                break;
            case 'l':
                list_employees_flag = true;
                break;
            case 'r':
                remove_employee_flag = true;
                remove_index = atoi(optarg);
                break;
            case 'u':
                update_employee_flag = true;
                update_arg = optarg;
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

    if (new_file_flag) {
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
    if (add_string) {
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
        add_employee(header, employees, add_string);
    }
    
    // List employees
    if (list_employees_flag) {
        list_employees(header, employees);
    }

    if (remove_employee_flag) {
        int remove_status = remove_employee(header, employees, remove_index);
        if (remove_status == STATUS_ERROR) {
            fprintf(stderr, "Failed to remove employee.\n");
            close(database_fd);
            return STATUS_ERROR;
        }
        
        // Reallocate memory to free unused space
        if (header->count > 0) {
            employees = realloc(employees, header->count * sizeof(struct employee_t));
            if (employees == NULL) {
                fprintf(stderr, "Failed to reallocate memory after removal.\n");
                close(database_fd);
                return STATUS_ERROR;
            }
        }
    }

    // Update an employee
    if (update_employee_flag && update_arg) {
        // Parse the update argument using the helper function
        int parse_status = parse_update_string(update_arg, &update_index, &update_string);
        if (parse_status == STATUS_ERROR) {
            fprintf(stderr, "Failed to parse update argument.\n");
            close(database_fd);
            return STATUS_ERROR;
        }
        
        int update_status = update_employee(header, employees, update_index, update_string);
        if (update_status == STATUS_ERROR) {
            fprintf(stderr, "Failed to update employee.\n");
            close(database_fd);
            return STATUS_ERROR;
        }
    }

    // Save the output file
    output_file(database_fd, header, employees);

    // Cleanup
    if (employees) {
        free(employees);
    }
    if (header) {
        free(header);
    }
    close(database_fd);

    return STATUS_SUCCESS;
}
