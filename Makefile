
CXX=g++
SRC=$(wildcard ./src/*.cpp)
HDR=$(wildcard ./src/*.h)
TEST=$(wildcard ./test/*.cpp)
BIN=$(patsubst ./test/%.cpp,%,$(TEST))

all: $(BIN)

%: ./test/%.cpp ./src/test_suite.cpp
	$(CXX) -g -Wall -Werror -std=c++11 -pthread -o ./bin/$@ $^
	ln -sf ./bin/$@ ./$@-bin

prepare:
	mkdir -p bin
	echo $(BIN)
