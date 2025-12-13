# Compiler
CC = gcc

# Compiler flags: C11, optimization, warnings, SDL2 includes
CFLAGS = -std=c11 -O2 -Wall -Wextra `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Build folder
BUILD_DIR = build

# Source files
SRC_ENGINE = src/core/engine.c
SRC_RENDERER = src/render/renderer.c
SRC_WORLD = src/world/world.c
SRC_MAIN = src/main.c

# Object files
OBJ_ENGINE = $(BUILD_DIR)/engine.o
OBJ_RENDERER = $(BUILD_DIR)/renderer.o
OBJ_WORLD = $(BUILD_DIR)/world.o
OBJ_MAIN = $(BUILD_DIR)/main.o

OBJ = $(OBJ_ENGINE) $(OBJ_RENDERER) $(OBJ_WORLD) $(OBJ_MAIN)

# Target binary
TARGET = $(BUILD_DIR)/granule

# Default target
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile individual source files into object files
$(OBJ_ENGINE): $(SRC_ENGINE)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_RENDERER): $(SRC_RENDERER)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_WORLD): $(SRC_WORLD)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_MAIN): $(SRC_MAIN)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
	@echo "Clean complete"

