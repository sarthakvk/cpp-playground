# Define the C++ compiler to use
CXX = g++

# Define the C++ standard to use
CXXFLAGS = -std=c++20 -pedantic -O3

# Define the target executable name
TARGET = intset

# Automatically find all .cpp files in the current directory
SRCS = $(wildcard *.cpp)
# Generate .o file names from .cpp files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
