all: game1 game2

game1: tet.o
	g++ -std=c++17 tet.o -o game1 -lncurses

game2: pac.o
	g++ -std=c++17 pac.o -o game2 -lncurses

tet.o: tet.cc AGE.h
	g++ -std=c++17 -g -c tet.cc

pac.o: pac.cc AGE.h
	g++ -std=c++17 -g -c pac.cc

.PHONY: clean

clean:
	rm game1 game2 *.o
