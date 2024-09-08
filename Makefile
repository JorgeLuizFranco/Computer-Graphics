all:
	g++ main.cpp -o main -lglfw -lGL -lGLEW -lGLU -lpthread -ldl

run:
	./main
