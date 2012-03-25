EXECUTABLE = sstest
DEPENDENCY = sstest.o 
CC = g++ -pthread
CCFLAGS= -Wall -c -g
OBJ_DIR=obj
SRC_DIR=src
EXE_DIR=build

$(EXECUTABLE) : $(DEPENDENCY)
	$(CC) $(DEPENDENCY) -o $(EXECUTABLE)
	
sstest.o : concurrent_sstest.cc ../$(SRC_DIR)/common/stepping_stone.h
	$(CC) $(CCFLAGS) concurrent_sstest.cc -o sstest.o
