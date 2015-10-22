CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: asteroids.cpp ppm.c log.c mytime.cpp
	g++ $(CFLAGS) asteroids.cpp log.c mytime.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

