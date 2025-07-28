#ifndef PARSE_H
#define PARSE_H

/**
 * @brief Defines the magic number for the database header.
 * 
 * This magic number is used to identify the database file format.
 */
#define HEADER_MAGIC 0x4c4c4144 // "LLAD" in ASCII

/**
 * @brief Represents the database header.
 * 
 * This structure holds metadata about the database file,
 * including magic number, version, employee count, and file size.
 */
struct dbheader_t {
	unsigned int magic;		// Magic number to identify the database file
	unsigned short version;	// Version of the database format
	unsigned short count;	// Number of employees in the database
	unsigned int filesize;	// Size of the database file
};

/**
 * @brief Represents an employee record.
 * 
 * This structure holds the employee's name, address, and hours worked.
 */
struct employee_t {
	char name[256];		// Employee's name
	char address[256];	// Employee's address
	unsigned int hours;	// Employee's hours worked
};

/**
 * @brief Create a db header object
 * 
 * @param fd The file descriptor of the database file.
 * @param headerOut Pointer to the output database header structure.
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 * @note This function allocates memory for the database header, initializes the header fields,
 * and writes the header to the file. It also handles memory allocation errors.
 */
int create_db_header(int fd, struct dbheader_t **headerOut);

/**
 * @brief Validates the database header.
 * 
 * @param fd The file descriptor of the database file.
 * @param headerOut Pointer to the output database header structure.
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 * @note This function validates the file descriptor, allocates memory for the database header,
 * reads the header from the file, unpacks the header fields from "network byte order"
 * to "host byte order", validates the header fields (version, magic number, filesize),
 * and sets the output header pointer.
 */
int validate_db_header(int fd, struct dbheader_t **headerOut);

/**
 * @brief Outputs the database file with the header and employee records.
 * 
 * @param fd The file descriptor of the database file.
 * @param employees Pointer to the array of employee structures.
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 * 
 * @note This function validates the file descriptor, calculates the total file size,
 * packs the header and employee fields from "host byte order" to "network byte order",
 * writes the header and employee records to the file, and truncates the file to the total
 * size if necessary.
 */
int output_file(int fd, struct dbheader_t *, struct employee_t *employees);

/**
 * @brief Reads employee records from a file.
 * 
 * @param fd The file descriptor of the database file.
 * @param employeesOut Pointer to the output array of employee structures.
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 * @note This function validates the file descriptor, allocates memory for the employees array,
 * reads the employee records from the file, unpacks the employee fields from "network byte order"
 * to "host byte order", and sets the output employees pointer.
 */
int read_employees(int fd, struct dbheader_t *, struct employee_t **employeesOut);

/**
 * @brief Lists all employees in the database.
 * 
 * @param db_header Pointer to the database header structure.
 * @param employees Pointer to the array of employee structures.
 * @note This function iterates through the employees and prints their details.
 * 
 * @example
 * list_employees(db_header, employees);
 */
void list_employees(struct dbheader_t *db_header, struct employee_t *employees);

/**
 * @brief Adds a new employee to the database.
 * 
 * @param db_header Pointer to database header structure.
 * @param employees Pointer to array of employee structures.
 * @param add_string The string containing employee information in the format "name,address,hours".
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 * 
 * @example
 * add_employee(db_header, employees, "John Doe,123 Main St,40");
 */
int add_employee(struct dbheader_t *db_header, struct employee_t *employees, char *add_string);

/**
 * @brief Removes an employee from the database.
 * 
 * @param db_header Pointer to database header structure.
 * @param employees Pointer to array of employee structures.
 * @param index The index of the employee to remove (0-based).
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 * @note Automatically shifts remaining employees down to fill the gap.
 * 
 * @example
 * remove_employee(db_header, employees, 2);
 */
int remove_employee(struct dbheader_t *db_header, struct employee_t *employees, int index);

/**
 * @brief Updates an existing employee's information.
 * 
 * Updates employee data at the specified index with new information.
 * The update string must be in the format: "name,address,hours"
 * 
 * @param db_header Pointer to database header structure.
 * @param employees The array of employees.
 * @param index The index of the employee to update (0-based).
 * @param update_string New employee data in format "name,address,hours".
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 * 
 * @example
 * update_employee(db_header, employees, 2, "John Doe,123 Main St,40");
 */
int update_employee(struct dbheader_t *db_header, struct employee_t *employees, int index, const char *update_string);

/**
 * @brief Parses the update string for employee information.
 * 
 * @param update_arg The update argument string containing index and data.
 * @param index Pointer to the index variable to store the extracted index.
 * @param data_string Pointer to the data string variable to store the extracted data.
 * @return STATUS_SUCCESS on success, STATUS_ERROR on failure.
 */
int parse_update_string(const char *update_arg, int *index, char **data_string);

#endif
