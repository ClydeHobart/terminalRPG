# Makefile for Terminal PRG directory
# Builds and compiles any non-modular programs into executables
#
# Zeke Baker | 20180630.1753 

NCT = ncursesTest
MPT = mapTest
YL = yoreland
COLT = colorTest

ALL = $(NCT) $(MPT) $(YL) $(COLT)

L = libs

FLAGS = -g
CFLAGS = -Wall -pedantic -std=c11 $(FLAGS)
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

$(YL): $(YL).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

$(COLT): $(COLT).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

clean:
	rm -f *.o *~ *.dSYM $(ALL)
	$(MAKE) -C $L clean