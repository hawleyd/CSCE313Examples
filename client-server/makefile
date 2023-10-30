CXX=g++
CXXFLAGS=-std=c++17


SRCS=tcp-Server.cpp tcp-Client.cpp getAddress.cpp
BINS=$(patsubst %.cpp,%.exe,$(SRCS))
DEPS=


all: clean $(BINS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.exe: %.cpp $(DEPS)
	$(CXX)  -o $(patsubst %.exe,%,$@) $(CXXFLAGS) $^ $(CXXFLAGS) 


.PHONY: clean

clean:
	rm -f getAddress 
	rm -f tcp-Server
	rm -f tcp-Client
