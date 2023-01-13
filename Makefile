CC = g++
CFLAGS = -std=c++17

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: build

build: $(OBJECTS)
	$(CC) $(OBJECTS) -o main.exe

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

clean:
	if exist $(BUILD_DIR) rmdir /Q /S $(BUILD_DIR)
	if exist main.exe del main.exe
