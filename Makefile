# Define the compiler
CC = gcc

# Define the source and header directories
SRC_DIR = src
INC_DIR = include

# Find all .c files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS += main.c

# Create a list of object files (.o) from the source files
OBJS = $(patsubst %.c, %.o, $(SRCS))

# Define the name of the final executable
TARGET = AESProgram

.PHONY: all clean

# The 'all' rule now handles the entire build and cleanup
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	@echo "Linking complete. Cleaning up object files..."
	rm -f $(OBJS)

# Rule to compile each .c file into an object file
# This is a pattern rule that applies to all .c files
%.o: %.c
	$(CC) -I$(INC_DIR) -c $< -o $@

# Rule to clean up only the final executable
clean:
	rm -f $(TARGET)