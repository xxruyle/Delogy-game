## Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++2a -g -O1 -Wall  -I./include

# Source and object files
SRC_DIR = ./src
OBJ_DIR = ./bin
INCLUDE_DIR = ./include
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Target executable
TARGET = $(OBJ_DIR)/Delogy.exe

# Build rule
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L./lib -lrayguidll -lraylib -lopengl32 -lgdi32 -lwinmm
	./bin/Delogy.exe

# Object file rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp ./include/*.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Phony targets
.PHONY: clean

# Clean rule
clean:
	rm -f $(OBJ_FILES) $(TARGET)

run: 
	./bin/Delogy.exe

