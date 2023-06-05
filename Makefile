CC = g++
CFLAGS = -Wall -Wextra -faligned-new -mavx

INCLUDE_DIR = include
SOURCE_DIR = src

INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)
SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SOURCE_DIR)/%.cpp, $(SOURCE_DIR)/objs/%.o, $(SOURCE_FILES))

EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(SOURCE_DIR)/objs/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_FILES)
	mkdir -p $(SOURCE_DIR)/objs
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

clean:
	rm -rf $(SOURCE_DIR)/objs/ $(EXECUTABLE)
