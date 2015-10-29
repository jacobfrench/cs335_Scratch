CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr 

all:	battlepong fonts.o fonttex.o libggfonts.a

battlepong:	battlepong.cpp ppm.cpp log.c mytime.cpp paddle.cpp player.cpp libggfonts.a
	g++ $(CFLAGS) battlepong.cpp  ppm.cpp log.c mytime.cpp jacobF.cpp  coryK.cpp brianC.cpp keithH.cpp paddle.cpp player.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -lalut -lopenal   -obattlepong

cory: coryK.cpp ppm.cpp
	g++ -c $(CFLAGS) coryK.cpp ppm.cpp $(LFLAGS)

brian: brianC.cpp
	g++ -c $(CFLAGS) brianC.cpp $(LFLAGS)

keith: keithH.cpp
	g++ -c $(CFLAGS) keithH.cpp $(LFLAGS)

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
	rm -f *.o
	rm -f libggfonts.a

