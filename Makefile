CC = g++
CFLAGS = -std=c++17

SRC_DIR = src
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: build

build: $(OBJECTS)
	$(CC) $(OBJECTS) -o filesystem

%.o: %.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJECTS) filesystem
