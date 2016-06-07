CC = g++
CFLAGS = -std=c++0x -isystem -Wall -g

EXEC_NAME = dula nula wordborhood

OBJ_FILES = levenshtein.o automaton.o

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

dula : $(OBJ_FILES) dula.o
	$(CC) -o $@ $(OBJ_FILES) dula.o

nula : $(OBJ_FILES) nula.o
	$(CC) -o $@ $(OBJ_FILES) nula.o

wordborhood : $(OBJ_FILES) wordborhood.o
	$(CC) -o $@ $(OBJ_FILES) wordborhood.o

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

