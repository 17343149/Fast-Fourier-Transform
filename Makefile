all: 
	g++ src/main.cpp -o build/main `pkg-config --cflags --libs opencv`