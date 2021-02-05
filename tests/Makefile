CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -pedantic -Wshadow -Wconversion 

all: 
	$(CXX) -c t.cpp -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN 
	$(CXX) -o t t.o -lboost_unit_test_framework ../Commodities/Commodity.o -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN -DBOOST_TEST_MODULE=Commodity

clean:
	rm -fr *.o t

