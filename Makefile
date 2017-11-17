CC=gcc 
CFLAGS=-std=c99 -lSDL2 
DEPS = chip8.h
OBJ = main.o CPU.o generalFunctions.o graphicsFunctions.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chip8: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)