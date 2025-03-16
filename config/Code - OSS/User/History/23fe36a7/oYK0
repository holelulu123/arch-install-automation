CXX=g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2

build/main: build/tile.o build/snake.o build/apple.o build/keyboard.o
	$(CXX) $(CXXFLAGS) build/keyboard.o build/tile.o build/apple.o build/snake.o -o build/main -lncurses 	

build/tile.o: src/tile.cpp 
	$(CXX) $(CXXFLAGS) -c src/tile.cpp -o build/tile.o

build/snake.o: src/snake.cpp include/snake.h
	$(CXX) $(CXXFLAGS) -c src/snake.cpp -o build/snake.o

build/apple.o: src/apple.cpp include/apple.h
	$(CXX) $(CXXFLAGS) -c src/apple.cpp -o build/apple.o

build/keyboard.o: src/keyboard.cpp include/keyboard.h
	$(CXX) $(CXXFLAGS) -c src/keyboard.cpp -I include/ -o build/keyboard.o -lncurses

clean:
	rm -f build/*