CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wshadow -Wconversion
TARGET = empire src_docs

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

empire: $(CXXSRC) $(CXXHEADERS)
	$(CXX) $(CXXFLAGS) -o target/empire $(CXXSRC)

src_docs: doxygen.conf empire
	doxygen doxygen.conf

clean:
	rm -fr target
	mkdir target

