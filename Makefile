CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: build

build: jacobF.cpp coryK.cpp ppm.c log.c mytime.cpp
	g++ $(CFLAGS) jacobF.cpp coryK.cpp log.c mytime.cpp ball.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o battlepong

clean:
	rm -f battlepong
	rm -f jacobF
	rm -f *.o

