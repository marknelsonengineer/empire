CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -pedantic -Wshadow -Wconversion 
TARGET = Commodity.o CommodityType.o

CXXSRC = main.cc World.cc
CXXHEADERS = World.hpp

all: $(TARGET)

Commodity.o: Commodity.cc
	$(CXX) $(CXXFLAGS) -c Commodity.cc

CommodityType.o: CommodityType.cc
	$(CXX) $(CXXFLAGS) -c CommodityType.cc

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr $(TARGET)

