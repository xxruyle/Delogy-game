HEADERS := $(wildcard include/*.hpp)
ENTHEADERS := $(wildcard include/entt/entity/registry.hpp)

debug: src/*.cpp $(HEADERS) $(ENTHEADERS)
	time g++ -std=c++2a src/*.cpp -o bin/Delogy.exe -g  -O1 -Wall -Wno-narrowing -Wno-unused-variable -Wno-enum-compare -I include/ -L /lib/ -lraylib -lrayguidll -lopengl32 -lgdi32 -lwinmm

	./bin/Delogy
build: src/*.cpp $(HEADERS)
	time g++ -std=c++2a src/*.cpp -o bin/Delogy.exe -g -O1 -Wall -Wno-narrowing -Wno-unused-variable -Wno-enum-compare -I include/ -L /lib/ -lraylib -lrayguidll -lopengl32 -lgdi32 -lwinmm
run: 
	./bin/Delogy
