CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr 

all:	battlepong

battlepong:	battlepong.cpp ppm.cpp log.c mytime.cpp paddle.cpp player.cpp
	g++ $(CFLAGS) battlepong.cpp  ppm.cpp log.c mytime.cpp jacobF.cpp  coryK.cpp brianC.cpp keithH.cpp paddle.cpp player.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -lalut -lopenal   -obattlepong

cory: coryK.cpp ppm.cpp
	g++ -c $(CFLAGS) coryK.cpp ppm.cpp $(LFLAGS)

brian: brianC.cpp
	g++ -c $(CFLAGS) brianC.cpp $(LFLAGS)

keith: keithH.cpp
	g++ -c $(CFLAGS) keithH.cpp $(LFLAGS)


clean:
	rm -f battlepong
	rm -f *.o

