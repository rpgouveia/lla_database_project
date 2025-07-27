#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x4c4c4144 // "LLAD" in ASCII

struct dbheader_t {
	unsigned int magic;		// Magic number to identify the database file
	unsigned short version;
	unsigned short count;
	unsigned int filesize;
};

struct employee_t {
	char name[256];
	char address[256];
	unsigned int hours;
};

int create_db_header(int fd, struct dbheader_t **headerOut);
int validate_db_header(int fd, struct dbheader_t **headerOut);
int output_file(int fd, struct dbheader_t *, struct employee_t *employees);
int read_employees(int fd, struct dbheader_t *, struct employee_t **employeesOut);
void list_employees(struct dbheader_t *db_header, struct employee_t *employees);
int add_employee(struct dbheader_t *db_header, struct employee_t *employees, char *addstring);
int remove_employee(struct dbheader_t *db_header, struct employee_t *employees, int index);
int update_employee(struct dbheader_t *db_header, struct employee_t *employees, int index, const char *update_string);
int parse_update_string(const char *update_arg, int *index, char **data_string);

#endif
