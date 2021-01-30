CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wshadow -Wconversion
TARGET = Commodity.o

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

Commodity.o: Commodity.cpp
	$(CXX) $(CXXFLAGS) -c Commodity.cpp

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr target
	mkdir target

