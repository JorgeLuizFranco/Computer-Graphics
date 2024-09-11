CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/usr/include -I/usr/local/include
LDFLAGS = -lglfw -lGL -lGLEW -lGLU -lpthread -ldl

all: main

main: main.o Pacman.o Star.o
	$(CXX) $(CXXFLAGS) main.o Pacman.o Star.o -o main $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Pacman.o: Pacman.cpp Pacman.h
	$(CXX) $(CXXFLAGS) -c Pacman.cpp

Star.o: Star.cpp Star.h
	$(CXX) $(CXXFLAGS) -c Star.cpp

clean:
	rm -f *.o main
