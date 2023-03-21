a3q6: main.o re.o
	g++ -std=c++14 main.o re.o -o a3q6

main.o: main.cc re.h
	g++ -std=c++14 -Wall -g -c main.cc

re.o: re.cc re.h
	g++ -std=c++14 -Wall -g -c re.cc

.PHONY: clean

clean:
	rm a3q6 *.o
