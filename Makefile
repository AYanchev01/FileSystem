CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -O2

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: build

build: checkdir $(OBJECTS)
	$(CC) $(OBJECTS) -o filesystem.exe

checkdir:
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

clean:
	if exist $(BUILD_DIR) rmdir /Q /S $(BUILD_DIR)
	if exist filesystem.exe del filesystem.exe