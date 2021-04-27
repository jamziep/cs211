# For building EECS 211 Homework 4
# You don’t need to edit this file, and you probably shouldn’t.


# This Makefile will automatically link against the libvc.so shared
# library if src/libvc.c doesn’t exist.
#
# However, you can also enable libvc.so temporarily without renaming
# or deleting src/libvc.c by passing defining a Make variable named
# USE_SHARED_LIBVC. You can define Make variables when running Make
# as extra arguments, like this:
#
#   % make test USE_SHARED_LIBVC=1
#
# Then Make will use the shared libvc.so in place of any src/libvc.c
# that might be available. Be careful, though, because the option won’t
# take effect until the programs are relinked. You can force a relink
# (and more) by running `make clean`.
ifeq ($(wildcard src/libvc.c),)
  USE_SHARED_LIBVC=1
endif


# The Make variable SIZE defined here specifies the value of
# MAX_CANDIDATES inside your C source. You don't need to edit this file
# to change this value. Instead, pass it as an additional command-line
# argument to Make, like so:
#
#   % make test SIZE=16
#
SIZE       ?= 16


# Suffixes for filenames
-Z          = -$(SIZE)
.O          = -$(SIZE).o

# If we’ve been asked to use the shared library, add it to the linker
# flags; otherwise, ask it to build a libvc.o and use that.
ifdef USE_SHARED_LIBVC
  LDFLAGS   += -lvc$(-Z)
  LIBVC.o    =
else
  LIBVC.o    = src/libvc$(.O)
endif

# C compiler to use:
CC         ?= cc
# Extra options for catching bad stuff:
SANFLAGS   += -fsanitize=address,undefined
# Flags for compiling individual files:
CFLAGS     += -g -O0 -std=c11 -DMAX_CANDIDATES=$(SIZE)
CFLAGS     += -pedantic-errors @.Wflags $(SANFLAGS)
# Flags for linking the final program:
LDFLAGS    += -l211 $(SANFLAGS)


# Test programs we can build:
TEST_EXES  = test_ballot test_ballot_box

# All programs we can build:
ALL_EXES   = $(TEST_EXES) irv

# Object files that we want to link into everything:
COMMON.o   = src/ballot$(.O) \
             src/ballot_box$(.O) \
             src/helpers$(.O) \
             $(LIBVC.o)


# Builds everything and runs ths tests.
test: $(ALL_EXES)
	@for prog in $(TEST_EXES); do \
	    echo "*** Running $$prog ***"; \
	    ./$$prog || echo "Error exit: $$?" ; \
	    echo; \
	done

# Builds everything but doesn’t run anything.
all: $(ALL_EXES)

# Creates a link from the main project directory to a binary in .bin/
# that has MAX_CANDIDATES set to SIZE:
%: .bin/%$(-Z)
	ln -sf $< $@

# The next three rules actually build the three binaries:

.bin/irv$(-Z): src/irv$(.O) $(COMMON.o)
	@mkdir -p .bin
	$(CC) -o $@ $^ $(LDFLAGS)

.bin/test_ballot$(-Z): test/test_ballot$(.O) $(COMMON.o)
	@mkdir -p .bin
	$(CC) -o $@ $^ $(LDFLAGS)

.bin/test_ballot_box$(-Z): test/test_ballot_box$(.O) $(COMMON.o)
	@mkdir -p .bin
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule for building object files with SIZE in the name:
%$(.O): %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

# When compiling C files in test/, `#include "..."` should look
# for header files in src/.
test/%$(.O): CPPFLAGS += -iquote src

# Dependencies (the files on the left get rebuilt from .c files when the
# .h files on the right change).
src/ballot$(.O): src/ballot.h src/helpers.h src/libvc.h
src/ballot_box$(.O): src/ballot.h src/ballot_box.h src/helpers.h src/libvc.h
src/helpers$(.O): src/helpers.h
src/irv$(.O): src/ballot.h src/ballot_box.h src/libvc.h
src/libvc$(.O): src/libvc.h
test/test_ballot$(.O): src/libvc.h src/ballot.h src/helpers.h
test/test_ballot_box$(.O): src/libvc.h src/helpers.h
test/test_ballot_box$(.O): src/ballot.h src/ballot_box.h

# Remove all build products so the next build will be from scratch.
# Useful if things get weird.
clean:
	$(RM) */*.o $(ALL_EXES)
	$(RM) -R .bin

# Tells make that these are fake targets, not actually files that we can
# build.
.PHONY: all test clean
