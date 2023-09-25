CXX=g++
CXXFLAGS=-std=c++17 -pedantic -Wall -Wextra -fno-omit-frame-pointer -lrt -pthread


SRCS=withSemaphore.cpp boundedBuffer.cpp
BINS=$(patsubst %.cpp,%.exe,$(SRCS))
DEPS=semaphore.cpp primes.cpp


all: clean $(BINS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.exe: %.cpp $(DEPS)
	$(CXX)  -o $(patsubst %.exe,%,$@) $(CXXFLAGS) $^ $(CXXFLAGS) 


.PHONY: clean

clean:
	rm -f withSemaphore
	rm -f boundedBuffer

