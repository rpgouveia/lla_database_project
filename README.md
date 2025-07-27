# Employee Database Management System

A simple command-line employee database management system written in C.

## Features

- Create new database file
- Add employees
- List all employees
- Remove employee by index
- Update employee information
- Data persistence using binary files

## Building

```bash
make
```

## Usage

```bash
./bin/dbview -f <database_file> [options]

Options:
  -n          Create new database file
  -f <path>   Specify database file path
  -a <data>   Add employee (format: name,address,hours)
  -l          List all employees
  -r <index>  Remove employee at index
  -u <data>   Update employee (format: index,name,address,hours)
```

## Examples

Create new database:
```bash
./bin/dbview -f employees.db -n
```

Add employee:
```bash
./bin/dbview -f employees.db -a "John Doe,123 Main St,40"
```

List employees:
```bash
./bin/dbview -f employees.db -l
```

Remove employee:
```bash
./bin/dbview -f employees.db -r 0
```

Update employee:
```bash
./bin/dbview -f employees.db -u "0,Jane Doe,456 Oak St,35"
```

## Project Structure

```
.
├── src/          # Source files
├── include/      # Header files
├── obj/          # Object files
└── bin/          # Binary output
```

## Requirements

- GCC compiler
- Make build system
- Linux operating system

## License

This project is licensed under the MIT License.