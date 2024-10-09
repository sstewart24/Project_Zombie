CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp matthewW.cpp lrafanan.cpp arodriguez3.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp matthewW.cpp lrafanan.cpp arodriguez3.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

