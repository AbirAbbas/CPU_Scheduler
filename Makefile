CC = gcc
CFLAGS = -Wall -std=c11 -g 

BIN = ./bin/
SRC = ./src/
INC = ./include/

all: $(BIN)structAPI.o $(BIN)simulationAPI.o $(BIN)LinkedListAPI.o
	$(CC) $(CFLAGS) -Iinclude $(SRC)main.c $(BIN)structAPI.o $(BIN)simulationAPI.o $(BIN)LinkedListAPI.o -o $(BIN)program

$(BIN)structAPI.o: $(SRC)structAPI.c
	$(CC) $(CFLAGS) -c $(SRC)structAPI.c -Iinclude -o $(BIN)structAPI.o
	
$(BIN)simulationAPI.o: $(SRC)simulationAPI.c
	$(CC) $(CFLAGS) -c $(SRC)simulationAPI.c -Iinclude -o $(BIN)simulationAPI.o
	
$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c
	$(CC) $(CFLAGS) -c $(SRC)LinkedListAPI.c -Iinclude -o $(BIN)LinkedListAPI.o

run: $(BIN)program
	$(BIN)program
	
clean: 
	rm -f $(BIN)*
