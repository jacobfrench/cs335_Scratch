CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: battlepong

battlepong: battlepong.cpp coryK.cpp ppm.c log.c mytime.cpp
	g++ $(CFLAGS) battlepong.cpp coryK.cpp log.c mytime.cpp jacobF.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -obattlepong
clean:
	rm -f battlepong
	rm -f *.o

