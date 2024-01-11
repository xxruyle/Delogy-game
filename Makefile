HEADERS := $(wildcard include/*.h)

debug: src/*.cpp $(HEADERS)
	g++ -std=c++2a src/*.cpp -o bin/main.exe -g -O1 -Wall -Wno-narrowing -Wno-unused-variable -Wno-enum-compare -I include/ -L /lib/ -lraylib -lrayguidll -lopengl32 -lgdi32 -lwinmm

	./bin/main
run: 
	./bin/main
