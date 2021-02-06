CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -pedantic -Wshadow -Wconversion 
TARGET = Commodity.o CommodityType.o EmpireExceptions.o

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

Commodity.o: Commodity.cpp Commodity.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c Commodity.cpp

CommodityType.o: CommodityType.cpp CommodityType.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c CommodityType.cpp

EmpireExceptions.o: EmpireExceptions.cpp EmpireExceptions.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c EmpireExceptions.cpp

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr $(TARGET) CommodityTests

CommodityTest.o: CommodityTest.cpp
	$(CXX) -c CommodityTest.cpp -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN -DBOOST_ENABLE_ASSERT_HANDLER

CommodityTests: CommodityTest.o Commodity.o EmpireExceptions.o
	$(CXX) -o CommodityTests Commodity.o CommodityTest.o EmpireExceptions.o -lboost_unit_test_framework -DBOOST_TEST_DYN_LINK -DBOOST_ENABLE_ASSERT_HANDLER

test: CommodityTests Commodity.o EmpireExceptions.o CommodityType.o
	./CommodityTests


