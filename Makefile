CXX = g++
CXXFLAGS = -std=c++17
TARGET = empire

CXXSRC = main.cpp World.cpp
CXXHEADERS = World.hpp

all: $(TARGET)

empire: $(CXXSRC) $(CXXHEADERS)
	$(CXX) $(CXXFLAGS) -o empire $(CXXSRC)

