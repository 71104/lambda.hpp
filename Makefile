CXXFLAGS+=-Wall -std=c++11 -pedantic -Ilib/

default : all

bin :
	mkdir -p bin

bin/test : bin lib/lambda.hpp src/test.cpp
	$(CXX) $(CXXFLAGS) -o bin/test src/test.cpp

test : bin/test
	bin/test

all : test

clean :
	rm -rf bin
