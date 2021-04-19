# For building CS 211 Homework 1
# You don’t need to edit this file, and you probably shouldn’t.

# Programs we can build:
EXES       = overlapped test_circle

# C compiler to use:
CC         ?= cc
# Extra options for catching bad stuff:
SANFLAGS   += -fsanitize=address,undefined
# Flags for compiling individual files:
CFLAGS     += -g -O0 -std=c11 -pedantic-errors @.Wflags $(SANFLAGS)
# Flags for linking the final program:
LDFLAGS    += -lm -l211 $(SANFLAGS)

# Builds both programs and runs the tests.
test: $(EXES)
	./test_circle

# Builds both programs but doesn’t run anything.
all: $(EXES)

# How to build the test program
test_circle: test/test_circle.o src/circle.o
	$(CC) -o $@ $^ $(LDFLAGS)

# How to build the client program
overlapped: src/overlapped.o src/circle.o
	$(CC) -o $@ $^ $(LDFLAGS)

# How to compile one .c file into a .o file
%.o: %.c
	$(CC) -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

# Removes all the build products
clean:
	rm -f $(EXES) */*.o

# Dependencies (when the file on the right changes then the file
# on the left needs to be rebuilt)
src/circle.o: src/circle.h
src/overlapped.o: src/circle.h
test/test_circle.o: src/circle.h

# Targets that are not actually files we can build:
.PHONY: all test clean
