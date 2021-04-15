#-----------------------------------------------------------------------------------#
# Makefile for 'coordinator' program                                                #
# Variables created for compiler and standard flags                                 #
# Also created variables for the separate directories to keep build location clean  #
#                                                                                   #
# Targets:                                                                          #                                              #
# coord: builds coord build objects in /                                            #
# sleeper: builds sleeper objects in /                                              #                                                    #
# clean: removes all objects, program and temporary files                           #
#                                                                                   #
# @author:  David Hines                                                             #
#-----------------------------------------------------------------------------------#

CC = gcc
CFLAGS = -Wall -std=c99 -g
TFLAG = -lpthread
SRC_DIR = ./
BUILD_DIR = ./
BIN_DIR = ./
SRC_LIST = $(wildcard $(SRC_DIR)*.c)
COORD_OBJ_LIST = $(BUILD_DIR)coordinator.o $(BUILD_DIR)sleeper.o
PROGRAM = $(wildcard $(BIN_DIR)*.exe)

# coordinator program target
coord: coordinator.o sleeper.o
	$(CC) $(CFLAGS) $(COORD_OBJ_LIST) -o $(BIN_DIR)coordinator $(TFLAG)

# object file targets
sleeper.o: $(SRC_DIR)sleeper.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)sleeper.c -o $(BUILD_DIR)sleeper.o

# clean target
clean:
	rm -f $(PROGRAM) $(COORD_OBJ_LIST)

