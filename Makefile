CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: battlepong fonts.o fonttex.o libggfonts.a

battlepong: battlepong.cpp ppm.c log.c mytime.cpp libggfonts.a
	g++ $(CFLAGS) battlepong.cpp log.c mytime.cpp jacobF.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -obattlepong

fonts.o: fonts.cpp fonts.h defs.h
	g++ -c -Wall -Wextra fonts.cpp

fonttex.o: fonttex.cpp
	g++ -c -Wall -Wextra fonttex.cpp

libggfonts.a: fonts.o fonttex.o
	ar -cvq libggfonts.a fonts.o fonttex.o
	rm libggfonts.a
	ar -cvq libggfonts.a fonts.o fonttex.o

clean:
	rm -f battlepong
	rm -f fonttex.o
	rm -f fonts.o
