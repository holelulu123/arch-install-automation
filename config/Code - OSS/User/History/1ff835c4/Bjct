# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Target executable
TARGET = main

# Source files
SRCS = src/apple.cpp src/snake.cpp src/tile.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c %< -o %@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	del $(TARGET_DEL) $(OBJS)
