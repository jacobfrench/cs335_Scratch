CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: battlepong

battlepong: battlepong.cpp ppm.c log.c mytime.cpp
	g++ $(CFLAGS) battlepong.cpp log.c mytime.cpp jacobF.cpp fonts.o fonttex.o -Wall -Wextra $(LFLAGS) -obattlepong
clean:
	rm -f battlepong

