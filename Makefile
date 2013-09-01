CFLAGS=-Wall -std=c++0x -pedantic -Isrc/ -Ibin/

all : bin bin/stdafx.gch src/lambda.hpp src/test.cpp
	g++ $(CFLAGS) -o bin/test src/test.cpp

bin :
	mkdir bin

bin/stdafx.gch : src/stdafx.h
	g++ $(CFLAGS) -c -o bin/stdafx.gch src/stdafx.h

clean :
	rm -rf bin
