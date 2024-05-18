# Makefile

# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin

# Source and object files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/warehouse

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean
#clean:
#	rm -f $(BINDIR)/*.o $(TARGET)

.PHONY: all clean
