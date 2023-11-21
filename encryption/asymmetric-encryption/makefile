CXX=g++
CXXFLAGS=-std=c++17 -lssl -lcrypto


SRCS=decrypt.cpp encrypt.cpp encryptFile
BINS=$(patsubst %.cpp,%.exe,$(SRCS))
DEPS=


all: clean $(BINS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.exe: %.cpp $(DEPS)
	$(CXX)  -o $(patsubst %.exe,%,$@) $(CXXFLAGS) $^ $(CXXFLAGS) 


.PHONY: clean

clean:
	rm -f decrypt
	rm -f encrypt
	rm -f encryptFile

