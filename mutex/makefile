CXX=g++
CXXFLAGS=-std=c++17 -pedantic -Wall -Wextra -fno-omit-frame-pointer -lrt -pthread


SRCS=noMutex.cpp withMutex.cpp threadStart.cpp
BINS=$(patsubst %.cpp,%.exe,$(SRCS))
DEPS=


all: clean $(BINS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.exe: %.cpp $(DEPS)
	$(CXX)  -o $(patsubst %.exe,%,$@) $(CXXFLAGS) $^ $(CXXFLAGS) 


.PHONY: clean

clean:
	rm -f noMutex
	rm -f threadStart
	rm -f withMutex

