# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g -std=c++11 -Iinclude

# Source, Object, and Binary directories
SRC_DIR = src
OBJ_DIR = bin
BIN_DIR = bin

# Executable name
TARGET = $(BIN_DIR)/myprogram

# Find source files, define object file names
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile C++ source to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Phony targets
.PHONY: all clean