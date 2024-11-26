CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: asteroids.cpp log.cpp mwehrmeyer.cpp lrafanan.cpp arodriguez3.cpp sstewart.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp mwehrmeyer.cpp lrafanan.cpp arodriguez3.cpp sstewart.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

