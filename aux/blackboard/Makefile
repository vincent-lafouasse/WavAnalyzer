CXX = c++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20

all: build
	./exe

build: exe

exe: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o exe

.PHONY: all build
