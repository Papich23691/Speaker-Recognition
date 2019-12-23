CC = gcc
SRC = src/*.c
OBJ = sr
INCLUDE_DIR = ./include
FLAGS = -lportaudio

all:
	@$(CC) $(SRC) $(FLAGS) -I$(INCLUDE_DIR) -w -o $(OBJ)
clean:
	@rm -rf ./sr 
