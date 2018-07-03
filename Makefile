# Makefile for Terminal PRG directory
# Builds and compiles any non-modular programs into executables
#
# Zeke Baker | 20180630.1753 

NCT = ncursesTest
MPT = mapTest

ALL = $(NCT) $(MPT)

L = libs

FLAGS =
CFLAGS = -Wall -pedantic -std=c11 $(FLAGS) -g
CC = gcc
MAKE = make
LIBS = $L/$L.a

.PHONY: all clean

all: $(LIBS) $(ALL)

$(LIBS):
	$(MAKE) -C $L

$(NCT): $(NCT).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

$(MPT): $(MPT).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

clean:
	rm -f *.o *~ *.dSYM $(ALL)
	$(MAKE) -C $L clean