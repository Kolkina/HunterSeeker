#Files to compile
OBJ = Game.cpp HuntingPolicy.cpp Prey.cpp

#Compiler Settings
CC = g++
CFLAGS = -w

#Executable name
OBJ_NAME = HunterSeeker

#Target for make
all : $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(ARG) -o $(OBJ_NAME)