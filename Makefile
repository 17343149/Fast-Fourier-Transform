TARGET = main

SRC_PATH = src/
BUILD_PATH = build/
INCLUDE_PATH = include/

all: $(TARGET) clean

$(TARGET): main.o common.o
	g++ $^ -o $(BUILD_PATH)$@ `pkg-config --cflags --libs opencv`

%.o: $(SRC_PATH)%.cpp
	g++ -c $^ -I $(INCLUDE_PATH) -o $@ `pkg-config --cflags --libs opencv`

clean:
	rm -rf *.o