###############################################################################
# Empire ][
#
# @file    Makefile
# @version 1.0 - Initial implementation
#
# @author Mark Nelson <marknels@hawaii.edu>
# @date   29 Jan 2021
# @copyright (c) 2021 Mark Nelson
###############################################################################

CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -pedantic -Wshadow -Wconversion
TARGET = Nation.o

all: $(TARGET)

Nation.o: Nation.cpp Nation.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c Nation.cpp

clean:
	rm -fr $(TARGET) CommodityTests

NationTest.o: NationTest.cpp Nation.hpp
	$(CXX) $(CXXFLAGS) -c NationTest.cpp -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN -DBOOST_ENABLE_ASSERT_HANDLER

NationTests: NationTest.o $(TARGET) 
	$(CXX) $(CXXFLAGS) -o NationTests NationTest.o $(TARGET) -lboost_unit_test_framework -DBOOST_TEST_DYN_LINK -DBOOST_ENABLE_ASSERT_HANDLER

test: NationTests $(TARGET)
	./NationTests
