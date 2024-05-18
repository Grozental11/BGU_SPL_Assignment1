# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

# Directories
SRC_DIR = src
BIN_DIR = bin
INC_DIR = include

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Executable
EXEC = $(BIN_DIR)/warehouse

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files to object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean the build directory (Windows version)
clean:
	if exist $(BIN_DIR)\*.o del /Q $(BIN_DIR)\*.o
	if exist $(EXEC) del /Q $(EXEC)

# Phony targets
.PHONY: all clean
