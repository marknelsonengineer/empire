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
CXXFLAGS = -std=c++20 -O3 -Wall -Wshadow -Wconversion
TARGET = empire src_docs

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

DOXYGEN_CONF = ./doxygen/doxygen.conf

all: $(TARGET)

EmpireExceptions.o: EmpireExceptions.cpp EmpireExceptions.hpp
	$(CXX) $(CXXFLAGS) -DBOOST_ENABLE_ASSERT_HANDLER -c EmpireExceptions.cpp

empire: $(CXXSRC) $(CXXHEADERS)
	$(CXX) $(CXXFLAGS) -o target/empire $(CXXSRC)

src_docs: $(DOXYGEN_CONF)
	doxygen $(DOXYGEN_CONF)
	# clear && make src_docs 2>&1 | fgrep "Commodity.cpp"

clean:
	rm -fr target
	mkdir target

