#include "World.hpp"

World::World(unsigned short width, unsigned short height) {
	this->width = width;
	this->height = height;
}

unsigned short World::getWidth() {
	return width;
}

unsigned short World::getHeight() {
	return height;
}

