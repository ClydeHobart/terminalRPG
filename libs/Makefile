# Makefile for libs directory
# Builds and compiles utility functions into a library.
#
# Zeke Baker | 20180313.2120 | Programs/libs/Makefile

FILE = file
ANIM = animations
PLAY = player
WRLD = world
COL = color

FLAGS = -g
CFLAGS = -Wall -pedantic -std=c11 $(FLAGS)
CC = gcc
MAKE = make
OBJS = $(FILE).o $(ANIM).o $(PLAY).o $(WRLD).o $(COL).o
LIBS = libs.a

.PHONY: clean

$(LIBS): $(OBJS)
	ar cr $@ $^
	rm -f *.o

$(FILE).o: $(FILE).h

$(ANIM).o: $(ANIM).h $(FILE).h
	$(CC) $(CFLAGS) -Ilibs -lncurses $(ANIM).c -c

$(PLAY).o: $(PLAY).h

$(WRLD).o: $(WRLD).h $(FILE).h $(PLAY).h
	$(CC) $(CFLAGS) -Ilibs -lncurses $(WRLD).c -c

$(COL).o: $(COL).h
	$(CC) $(CFLAGS) -lncurses $(COL).c -c

clean:
	rm -f *.o *.a *~  *.dSYM