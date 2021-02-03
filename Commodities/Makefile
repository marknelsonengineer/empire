CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -pedantic -Wshadow -Wconversion 
TARGET = Commodity.o CommodityType.o

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

Commodity.o: Commodity.cpp
	$(CXX) $(CXXFLAGS) -c Commodity.cpp

CommodityType.o: CommodityType.cpp
	$(CXX) $(CXXFLAGS) -c CommodityType.cpp

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr $(TARGET)

