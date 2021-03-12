###############################################################################
# Empire ][
#
# @file    lib/Makefile
# @version 1.0 - Initial implementation
#
# @author Mark Nelson <marknels@hawaii.edu>
# @date   29 Jan 2021
# @copyright (c) 2021 Mark Nelson
###############################################################################

CXX      = g++
CXXFLAGS = -std=c++20    \
           -O3           \
           -Wall         \
           -pedantic     \
           -Wshadow      \
           -Wconversion

AR       = ar
TARGET   = libempire.a


BOOST_FLAGS          = -DBOOST_ENABLE_ASSERT_HANDLER
BOOST_TEST_CXX_FLAGS = -DBOOST_TEST_DYN_LINK          \
                       -DBOOST_TEST_MAIN
BOOST_TEST_LD_FLAGS  = -lboost_unit_test_framework    \
                       -DBOOST_TEST_DYN_LINK          \
                       -DBOOST_ENABLE_ASSERT_HANDLER
CXX_TEST_FLAGS       = $(CXXFLAGS) $(BOOST_FLAGS) $(BOOST_TEST_CXX_FLAGS)


all: $(TARGET)

EmpireExceptions.o: EmpireExceptions.cpp EmpireExceptions.hpp
	$(CXX) $(CXXFLAGS) -c EmpireExceptions.cpp

EmpireExceptionsTest.o: EmpireExceptionsTest.cpp EmpireExceptions.hpp
	$(CXX) $(CXX_TEST_FLAGS) -c EmpireExceptionsTest.cpp

EmpireExceptionsTest: EmpireExceptions.o EmpireExceptionsTest.o
	$(CXX) $(CXX_TEST_FLAGS) $(BOOST_TEST_LD_FLAGS) -o EmpireExceptionsTest $^ 

$(TARGET): EmpireExceptions.o
	$(AR) -rsv $(TARGET) $^

test: EmpireExceptionsTest $(TARGET)
	./EmpireExceptionsTest
	
clean:
	rm -fr *.o $(TARGET) EmpireExceptionsTest
