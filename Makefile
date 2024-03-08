## Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++2a -g -O1 -Wall -Wno-unused-variable  -I./include
CXXFLAGS_STATIC = -std=c++2a -static -static-libgcc -static-libstdc++ -g -O1 -Wall -Wno-unused-variable  -I./include

# Source and object files
SRC_DIR = ./src
INCLUDE_DIR = ./include
OBJ_DIR = ./bin
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
HEADER_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Target executable
TARGET = $(OBJ_DIR)/Delogy.exe
STATIC_TARGET = $(OBJ_DIR)/DelogyStatic.exe 

# Build rule
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L./lib -lrayguidll -lraylib -lopengl32 -lgdi32 -lwinmm
	./bin/Delogy.exe

$(STATIC_TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS_STATIC) -o $@ $^ -L./lib -lrayguidll -lraylib -lopengl32 -lgdi32 -lwinmm

# Object file rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Phony targets
.PHONY: clean cleanstatic run 

# Clean rule
clean:
	rm -f $(OBJ_FILES) $(TARGET)

cleanstatic: 
	rm ./bin/DelogyStatic.exe


run: 
	$(TARGET)



