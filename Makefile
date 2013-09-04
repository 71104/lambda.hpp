CPPFLAGS=-Wall -std=c++0x -pedantic -Isrc/ -Ibin/

all : bin bin/stdafx.gch src/lambda.hpp src/test.cpp
	$(CXX) $(CPPFLAGS) -o bin/test src/test.cpp

bin :
	mkdir bin

bin/stdafx.gch : src/stdafx.h
	$(CXX) $(CPPFLAGS) -c -o bin/stdafx.gch src/stdafx.h

clean :
	rm -rf bin
