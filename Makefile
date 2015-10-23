CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: jacobF

jacobF: jacobF.cpp ppm.c log.c mytime.cpp
	g++ $(CFLAGS) jacobF.cpp log.c mytime.cpp ball.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -ojacobF
clean:
	rm -f jacobF
	rm -f *.o

