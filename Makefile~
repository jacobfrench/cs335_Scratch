CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: battlepong fonts.o fonttex.o

battlepong: battlepong.cpp ppm.c log.c mytime.cpp fonts.o fonttex.o
	g++ $(CFLAGS) battlepong.cpp log.c mytime.cpp jacobF.cpp fonts.o fonttex.o -Wall -Wextra $(LFLAGS) -obattlepong

fonts.o: fonts.cpp fonts.h defs.h
	g++ -c -Wall -Wextra fonts.cpp

fonttex.o: fonttex.cpp
	g++ -c -Wall -Wextra fonttex.cpp

clean:
	rm -f battlepong
	rm -f fonttex.o
	rm -f fonts.o

