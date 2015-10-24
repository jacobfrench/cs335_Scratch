CFLAGS = -I ./include
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
LFLAGS =  -lX11 -lGLU -lglut -lGL -I/usr/X11R6/include -L/usr/X11R6/lib -pthread -lm

all:	jacobF

jacobF:	jacobF.cpp ppm.c log.c mytime.cpp
	g++ $(CFLAGS) jacobF.cpp log.c mytime.cpp ball.cpp fonts.o fonttex.o -Wall -Wextra $(LFLAGS) -ojacobF
clean:
	rm -f jacobF
