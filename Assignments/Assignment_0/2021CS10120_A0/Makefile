# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -g -std=c++17  # Optimization, debugging, and C++17 standard
TARGET = main
SRC = main.cpp

# Default target to build the binary
all: $(TARGET)

# Rule to compile and create the binary
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean up build files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
