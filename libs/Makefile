# Makefile for libs directory
# Builds and compiles utility functions into a library.
#
# Zeke Baker | 20180313.2120 | Programs/libs/Makefile

FILE = file
ANIM = animations

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 $(FLAGS) -g
CC = gcc
MAKE = make
OBJS = $(FILE).o $(ANIM).o
LIBS = libs.a

.PHONY: clean

$(LIBS): $(OBJS)
	ar cr $@ $^
	rm -f *.o

$(FILE).o: $(FILE).h

$(ANIM).o: $(ANIM).h $(FILE).h
	$(CC) $(CFLAGS) -Ilibs -lncurses $(ANIM).c -c

clean:
	rm -f *.o *.a *~  *.dSYM
