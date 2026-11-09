# Compiler and compiler options.
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude

# The executable contains every implementation in src/.
TARGET := smart_campus
SOURCES := $(wildcard src/*.cpp)

.PHONY: all run clean

# Default target: build the program.
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

# Build first, then start the program.
run: $(TARGET)
	./$(TARGET)

# Remove the generated executable.
clean:
	$(RM) $(TARGET) $(TARGET).exe
