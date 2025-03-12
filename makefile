# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra
# Target executable
TARGET = student_records
# Source files
SRCS = main.cpp CourseInfo.cpp Node.cpp AVLTree.cpp FileHandler.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Header files
HEADERS = courseinfo.h \
          filehandler.h \
          node.h \
          node_operations.h

# Default target
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile .cpp files into .o files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable with default input file
run: $(TARGET)
	./$(TARGET) input.txt

# Rule to run with roll number key
run-rollno: $(TARGET)
	./$(TARGET) input.txt rollno

# Rule to run with CGPA key
run-cgpa: $(TARGET)
	./$(TARGET) input.txt cgpa

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run run-rollno run-cgpa clean