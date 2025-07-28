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
#include "validation.h"

/**
 * @brief Implements the parse update functionality.
 * 
 * Implementation details:
 * - Validates the index and update string format.
 * - Uses strncpy to safely copy strings.
 * - Uses strtok to parse the update string.
 * - Uses atoi to convert the hours string to an integer.
 * - Uses strchr to find the comma in the original string.
 */
int parse_update_string(const char *update_arg, int *index, char **data_string) {
    // Validate input parameters
    if (!update_arg || !index || !data_string) {
        fprintf(stderr, "Invalid parameters for update parsing\n");
        return STATUS_ERROR;
    }

    // Create a copy of the string for parsing
    char temp_string[512];
    strncpy(temp_string, update_arg, sizeof(temp_string) - 1);
    temp_string[sizeof(temp_string) - 1] = '\0';
    
    // Extract the index
    char *idx_str = strtok(temp_string, ",");
    if (!idx_str) {
        fprintf(stderr, "Invalid update format. Expected: index,name,address,hours\n");
        return STATUS_ERROR;
    }
    
    *index = atoi(idx_str);
    
    // Find the comma in the original string and get the data part
    const char *comma_pos = strchr(update_arg, ',');
    if (!comma_pos) {
        fprintf(stderr, "Invalid update format. Missing data after index\n");
        return STATUS_ERROR;
    }
    
    // Point to the character after the comma
    *data_string = (char *)(comma_pos + 1);
    
    // Validate that we have some data
    if (strlen(*data_string) == 0) {
        fprintf(stderr, "Invalid update format. No data provided\n");
        return STATUS_ERROR;
    }
    
    return STATUS_SUCCESS;
}

/**
 * @brief Implementation of employee update functionality.
 * 
 * Implementation details:
 * - Validates the index and update string format.
 * - Uses strncpy to safely copy strings.
 * - Uses strtok to parse the update string.
 * - Updates the employee's name, address, and hours.
 */
int update_employee(struct dbheader_t *db_header, struct employee_t *employees, int index, const char *update_string) {
    // Validate parameters
    if (!db_header || !employees || !update_string) {
        return handle_file_error("Invalid parameters");
    }

    // Check if index is valid
    if (index < 0 || index >= db_header->count) {
        fprintf(stderr, "Invalid index: %d. Must be between 0 and %d\n", 
                index, db_header->count - 1);
        return STATUS_ERROR;
    }

    // Create a copy of update_string since strtok modifies the string
    char update_data[256];
    strncpy(update_data, update_string, sizeof(update_data) - 1);
    update_data[sizeof(update_data) - 1] = '\0';

    // Parse the update string
    char *name = strtok(update_data, ",");
    char *address = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");

    if (!name || !address || !hours) {
        fprintf(stderr, "Invalid update format. Expected: name,address,hours\n");
        return STATUS_ERROR;
    }

    // Update the employee data
    strncpy(employees[index].name, name, sizeof(employees[index].name) - 1);
    employees[index].name[sizeof(employees[index].name) - 1] = '\0';
    
    strncpy(employees[index].address, address, sizeof(employees[index].address) - 1);
    employees[index].address[sizeof(employees[index].address) - 1] = '\0';
    
    employees[index].hours = atoi(hours);

    printf("Employee at index %d updated successfully\n", index);
    return STATUS_SUCCESS;
}

/**
 * @brief Removes an employee from the database.
 * 
 * Implementation details:
 * - Validates the index.
 * - Checks if there are employees to remove.
 * - Moves all employees after the index one position back.
 * - Decrements the employee count.
 * - Handles memory allocation and error checking.
 */
int remove_employee(struct dbheader_t *db_header, struct employee_t *employees, int index) {
    // Check if the index is valid
    if (index < 0 || index >= db_header->count) {
        fprintf(stderr, "Invalid index: %d. Must be between 0 and %d\n", index, db_header->count - 1);
        return STATUS_ERROR;
    }

    // Check if there are employees to remove
    if (db_header->count == 0) {
        fprintf(stderr, "There are no employees to remove\n");
        return STATUS_ERROR;
    }

    // Move all employees after the index one position back
    for (int i = index; i < db_header->count - 1; i++) {
        // Copy the next employee to the current position
        strcpy(employees[i].name, employees[i + 1].name);
        strcpy(employees[i].address, employees[i + 1].address);
        employees[i].hours = employees[i + 1].hours;
    }

    // Decrement the employee count
    db_header->count--;

    printf("Employee at index %d removed successfully\n", index);
    return STATUS_SUCCESS;
}

/**
 * @brief Lists all employees in the database.
 * 
 * Implementation details:
 * - Iterates through the employees array.
 * - Prints each employee's details.
 */
void list_employees(struct dbheader_t *db_header, struct employee_t *employees) {
    for (int i = 0; i < db_header->count; i++) {
        printf("Employee %d:\n", i + 1);
        printf("\tName: %s\n", employees[i].name);
        printf("\tAddress: %s\n", employees[i].address);
        printf("\tHours: %u\n", employees[i].hours);
    }
}

/**
 * @brief Adds a new employee to the database.
 * 
 * Implementation details:
 * - Validates the input string format.
 * - Uses strtok to parse the input string.
 * - Uses strncpy to safely copy strings.
 * - Increments the employee count in the header.
 * - Handles memory allocation and error checking.
 */
int add_employee(struct dbheader_t *db_header, struct employee_t *employees, char *add_string) {
    printf("%s\n", add_string);

    char *name = strtok(add_string, ",");
    char *address = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");
    printf("Name: %s, Address: %s, Hours: %s\n", name, address, hours);

    // String copying with size limit
    strncpy(employees[db_header->count-1].name, name, sizeof(employees[db_header->count-1].name));
    strncpy(employees[db_header->count-1].address, address, sizeof(employees[db_header->count-1].address));
    employees[db_header->count-1].hours = atoi(hours);

    return STATUS_SUCCESS;
}

/**
 * @brief Reads employee records from a file.
 * 
 * Implementation details:
 * - Validates the file descriptor.
 * - Allocates memory for the employees array.
 * - Reads the employee records from the file.
 * - Unpacks the employee fields from "network byte order" to "host byte order".
 * - Sets the output employees pointer.
 */
int read_employees(int fd, struct dbheader_t *db_header, struct employee_t **employeesOut) {
    // Validate the file descriptor
    if (fd < 0) {
        return handle_file_error("Invalid file descriptor");
    }

    // Gather the number of employees from the database header
    int count = db_header->count;

    // Allocate memory for the employees array
    struct employee_t *employees = calloc(count, sizeof(struct employee_t));
    if (employees == NULL) {
        return handle_file_error("Error allocating memory for employees");
    }

    // Read the employees from the file
    read(fd, employees, count * sizeof(struct employee_t));
    for (int i = 0; i < count; i++) {
        // Unpack the employee fields from "network byte order" to "host byte order"
        // Note: name and address are already in host byte order since they are character arrays
        employees[i].hours = ntohl(employees[i].hours);
    }

    // Set the output employees pointer
    *employeesOut = employees;
    return STATUS_SUCCESS;
}

/**
 * @brief Outputs the database file with the header and employee records.
 * 
 * Implementation details:
 * - Validates the file descriptor.
 * - Packs the header fields from "host byte order" to "network byte order".
 * - Writes the header and employee records to the file.
 * - Truncates the file to the total size.
 */
int output_file(int fd, struct dbheader_t *db_header, struct employee_t *employees) {
    // Validate the file descriptor
    if (fd < 0) {
        return handle_file_error("Invalid file descriptor");
    }

    // Get the actual count of employees
    int real_count = db_header->count;

    // Calculate the total file size
    size_t total_size = sizeof(struct dbheader_t) + sizeof(struct employee_t) * real_count;

    // Pack the header fields from "host byte order" to "network byte order" (from memory to disc)
    db_header->version = htons(db_header->version);
    db_header->count = htons(db_header->count);
    db_header->magic = htonl(db_header->magic);
    db_header->filesize = htonl(total_size);

    // Write the header to the file (for now)
    lseek(fd, 0, SEEK_SET);
    write(fd, db_header, sizeof(struct dbheader_t));
    
    for (int i = 0; i < real_count; i++) {
        // Pack the employee fields from "host byte order" to "network byte order"
        // Note: name and address are already in host byte order since they are character arrays
        employees[i].hours = htonl(employees[i].hours);
        write(fd, &employees[i], sizeof(struct employee_t));
    }

    // Truncate the file to the total size (Essential when employees are removed)
    if (ftruncate(fd, total_size) == -1) {
        perror("Error truncating file");
        return STATUS_ERROR;
    }
    
    return STATUS_SUCCESS;
}	

/**
 * @brief Validates the database header.
 * 
 * Implementation details:
 * - Validates the file descriptor.
 * - Allocates memory for the database header.
 * - Reads the header from the file.
 * - Unpacks the header fields from "network byte order" to "host byte order".
 * - Validates the header fields (version, magic number, filesize).
 * - Sets the output header pointer.
 */
int validate_db_header(int fd, struct dbheader_t **headerOut) {
    // Validate the file descriptor
    if (fd < 0) {
        return handle_file_error("Invalid file descriptor");
    }

    // Allocate memory for the database header
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL) {
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

/**
 * @brief Create a db header object
 * 
 * Implementation details:
 * - Allocates memory for the database header.
 * - Initializes the header fields.
 * - Writes the header to the file.
 * - Handles memory allocation errors.
 */
int create_db_header(int fd, struct dbheader_t **headerOut) {
    // Allocate memory for the database header
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL){
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


