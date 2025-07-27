TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -f ./mynewdb.db -n 
	./$(TARGET) -f ./mynewdb.db -a "Timmy H.,123 Sheshire Ln.,120"
	./$(TARGET) -f ./mynewdb.db -a "John Doe,456 Elm St.,40"
	./$(TARGET) -f ./mynewdb.db -a "Jane Smith,789 Oak Ave.,30"
	./$(TARGET) -f ./mynewdb.db -a "Alice Johnson,321 Pine Rd.,50"
	./$(TARGET) -f ./mynewdb.db -l

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude -g


