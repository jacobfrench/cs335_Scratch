CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all:	battlepong

battlepong:	battlepong.cpp ppm.cpp log.c mytime.cpp paddle.cpp
	g++ $(CFLAGS) battlepong.cpp ppm.cpp log.c mytime.cpp jacobF.cpp paddle.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -obattlepong

clean:
	rm -f battlepong
	rm -f *.o

