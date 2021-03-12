###############################################################################
# Empire ][
#
# @file    Commodities/Makefile
# @version 1.0 - Initial implementation
#
# @author Mark Nelson <marknels@hawaii.edu>
# @date   29 Jan 2021
# @copyright (c) 2021 Mark Nelson
###############################################################################

CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -pedantic -Wshadow -Wconversion 
TARGET = Commodity.o EmpireExceptions.o

all: $(TARGET)

Commodity.o: Commodity.cpp Commodity.hpp EmpireExceptions.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c Commodity.cpp

clean:
	rm -fr $(TARGET) CommodityTests

CommodityTest.o: CommodityTest.cpp EmpireExceptions.hpp Commodity.hpp
	$(CXX) $(CXXFLAGS) -c CommodityTest.cpp -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN -DBOOST_ENABLE_ASSERT_HANDLER

CommodityTests: CommodityTest.o $(TARGET) 
	$(CXX) $(CXXFLAGS) -o CommodityTests CommodityTest.o $(TARGET) -lboost_unit_test_framework -DBOOST_TEST_DYN_LINK -DBOOST_ENABLE_ASSERT_HANDLER

test: CommodityTests $(TARGET)
	./CommodityTests
