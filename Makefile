CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wshadow -Wconversion
TARGET = empire src_docs

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

DOXYGEN_CONF = ./doxygen/doxygen.conf

all: $(TARGET)

empire: $(CXXSRC) $(CXXHEADERS)
	$(CXX) $(CXXFLAGS) -o target/empire $(CXXSRC)

src_docs: $(DOXYGEN_CONF)
	doxygen $(DOXYGEN_CONF)
	# clear && make src_docs 2>&1 | fgrep "Commodity.cpp"

clean:
	rm -fr target
	mkdir target

