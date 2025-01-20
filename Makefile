# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Output binary
TARGET = $(BUILD_DIR)/hash_table_program

# Default rule: build the target
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Rule to compile .c files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
