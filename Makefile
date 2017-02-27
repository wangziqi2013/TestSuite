
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
	$(CXX) -g -Wall -Werror -I./src/ -I/usr/include/python2.7/ -std=c++11 -pthread -o ./bin/$@ $^ -lpython2.7
	ln -sf ./bin/$@ ./$@-bin

prepare:
	mkdir -p bin
	mkdir -p build
	echo $(BIN)
