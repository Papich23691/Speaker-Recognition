CC = gcc
SRC = src/main.c src/sound.c src/record.c
OBJ = mfcc
INCLUDE_DIR = ./include
FLAGS += -lportaudio

all:
	@$(CC) $(SRC) $(FLAGS) -I$(INCLUDE_DIR) -w -o $(OBJ)
clean:
	@rm -rf ./run ./run.dSYM 
