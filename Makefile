HEADERS := $(wildcard include/*.h)

debug: src/*.cpp $(HEADERS)
	g++ src/*.cpp -o bin/main.exe -g -O1 -Wall  -I include/ -L /lib/ -lraylib -lopengl32 -lgdi32 -lwinmm 
run: 
	./bin/main