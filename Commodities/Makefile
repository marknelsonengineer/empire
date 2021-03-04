CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -pedantic -Wshadow -Wconversion 
TARGET = Commodity.o EmpireExceptions.o # CommodityType.o

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

#CommodityType.o: CommodityType.cpp CommodityType.hpp
#	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c CommodityType.cpp

Commodity.o: Commodity.cpp Commodity.hpp EmpireExceptions.hpp CommodityType.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c Commodity.cpp

EmpireExceptions.o: EmpireExceptions.cpp EmpireExceptions.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c EmpireExceptions.cpp

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr $(TARGET) CommodityTests

CommodityTest.o: CommodityTest.cpp EmpireExceptions.hpp Commodity.hpp
	$(CXX) $(CXXFLAGS) -c CommodityTest.cpp -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN -DBOOST_ENABLE_ASSERT_HANDLER

CommodityTests: CommodityTest.o $(TARGET) 
	$(CXX) $(CXXFLAGS) -o CommodityTests CommodityTest.o $(TARGET) -lboost_unit_test_framework -DBOOST_TEST_DYN_LINK -DBOOST_ENABLE_ASSERT_HANDLER

test: CommodityTests Commodity.o EmpireExceptions.o CommodityType.o
	./CommodityTests


