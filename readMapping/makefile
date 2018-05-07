# build an executable for the program

#define the compiler to use (gcc, g++)
CC = g++

#define compiler flags
CFLAGS = -ggdb -Wall -std=c++11

#the build target executable
#TARGET = map

SRC = ./src/
OBJS = ./objs/

#define make rules

default: map

map: $(OBJS)alignmentTable.o $(OBJS)suffixNode.o $(OBJS)suffixTree.o $(OBJS)main.o
	$(CC) $(CFLAGS) -o map $(OBJS)alignmentTable.o $(OBJS)suffixNode.o $(OBJS)suffixTree.o $(OBJS)main.o

$(OBJS)alignmentTable.o: $(SRC)alignmentTable.cpp $(SRC)alignmentCell.h $(SRC)alignmentTable.h
	$(CC) $(CFLAGS) -c -o $(OBJS)alignmentTable.o $(SRC)alignmentTable.cpp

$(OBJS)suffixNode.o: $(SRC)suffixNode.cpp $(SRC)suffixNode.h
	$(CC) $(CFLAGS) -c -o $(OBJS)suffixNode.o $(SRC)suffixNode.cpp

$(OBJS)suffixTree.o: $(SRC)suffixTree.cpp $(SRC)suffixNode.h $(SRC)suffixTree.h
	$(CC) $(CFLAGS) -c -o $(OBJS)suffixTree.o $(SRC)suffixTree.cpp

$(OBJS)main.o: $(SRC)main.cpp
	$(CC) $(CFLAGS) -c -o $(OBJS)main.o $(SRC)main.cpp

clean:
	$(RM) map *.o

