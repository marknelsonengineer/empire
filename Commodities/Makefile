CXX = clang++
CXXFLAGS = -Wall -std=c++2a -stdlib=libc++ -c -pedantic -Wno-unused-private-field
TARGET = Commodity.o CommodityType.o
INCLUDE_DIRS = -I /usr/include/c++/10 -I /usr/include/c++/10/x86_64-redhat-linux

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

Commodity.o: Commodity.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c Commodity.cpp

CommodityType.o: CommodityType.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c CommodityType.cpp

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr $(TARGET)

