
CXX=g++
SRC=$(wildcard ./src/*.cpp)
HDR=$(wildcard ./src/*.h)
TEST=$(wildcard ./test/*.cpp)
BIN=$(patsubst ./test/%.cpp,%,$(TEST))

all: $(BIN)
	./arg_test-bin
	key1=value1 key2=2 key3=asdf ./envp_test-bin
	./intskey_test-bin

%: ./test/%.cpp ./src/test_suite.cpp ./src/plot_suite.cpp
	$(CXX) -g -Wall -Werror -I./src/ -std=c++11 -pthread -o ./bin/$@ $^
	ln -sf ./bin/$@ ./$@-bin

prepare:
	mkdir -p bin
	mkdir -p build
	echo $(BIN)
