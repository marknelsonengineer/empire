CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -pedantic -Wshadow -Wconversion 
TARGET = Commodity.o CommodityType.o

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

Commodity.o: Commodity.cpp Commodity.hpp
	$(CXX) $(CXXFLAGS) -c Commodity.cpp

CommodityType.o: CommodityType.cpp CommodityType.hpp
	$(CXX) $(CXXFLAGS) -c CommodityType.cpp

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr $(TARGET)

