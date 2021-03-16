###############################################################################
# Empire ][
#
# Common Make include file
#
# @file    Common.mk
# @version 1.0 - Initial implementation
#
# @author Mark Nelson <marknels@hawaii.edu>
# @date   13 Mar 2021
# @copyright (c) 2021 Mark Nelson
###############################################################################

CXX      = g++
CXXFLAGS = -std=c++20    \
           -O3           \
           -Wall         \
           -pedantic     \
           -Wshadow      \
           -Wconversion

LDFLAGS  = -L../lib      \
           -lempire

AR       = ar

BOOST_FLAGS          = -DBOOST_ENABLE_ASSERT_HANDLER
BOOST_TEST_CXX_FLAGS = -DBOOST_TEST_DYN_LINK          \
                       -DBOOST_TEST_MAIN
BOOST_TEST_LD_FLAGS  = -lboost_unit_test_framework    \
                       -DBOOST_TEST_DYN_LINK          \
                       -DBOOST_ENABLE_ASSERT_HANDLER
CXX_TEST_FLAGS       = $(CXXFLAGS) $(BOOST_FLAGS) $(BOOST_TEST_CXX_FLAGS)


# The following compiler templates assume that...
#   $(TARGETS) = A list of .o targets for empire
#   $(TESTS)   = A list of .o targets for unit tests

$(TARGETS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(BOOST_FLAGS) -o $@ $<
	
# For each Boost Test target, there is one .cpp.  Create one .o and one executable.
# If we ever build a combined test, we can incorporate all of the .o files
# into one combined test.
$(TESTS): %: %.cpp $(TARGETS)
	$(CXX) -c -o $@.o $(CXX_TEST_FLAGS) $<
	$(CXX)    -o $@   $(CXX_TEST_FLAGS) $< $(TARGETS) $(LDFLAGS) $(BOOST_TEST_LD_FLAGS)

test: $(TARGETS) $(TARGET) $(TESTS) 
	@ for t in $(TESTS);  do \
		./$$t;                \
	done

clean:
	rm -fr *.o $(TARGETS) $(TARGET) $(TESTS) 

.PHONY: all
.PHONY: test
.PHONY: clean
