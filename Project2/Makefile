CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
LIBS = -lGL -lGLEW -lglfw -lassimp
INCLUDES = -I/usr/include -I/usr/local/include

all: main

main: main.o objloader.o
	$(CXX) -o main main.o objloader.o $(LIBS)

main.o: main.cpp objloader.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp $(INCLUDES)

objloader.o: objloader.cpp objloader.hpp
	$(CXX) $(CXXFLAGS) -c objloader.cpp $(INCLUDES)

clean:
	rm -f *.o main
