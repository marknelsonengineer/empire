#include "World.hpp"

/// Constructs the World
World::World(unsigned short world_width, unsigned short world_height) {
	this->width = world_width;
	this->height = world_height;
}

/// Return the width of the world
unsigned short World::getWidth() {
	return width;
}

/// Return the height of the world
unsigned short World::getHeight() {
	return height;
}

