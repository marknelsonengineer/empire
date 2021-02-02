#ifndef WORLD_H
#define WORLD_H

/// The World, the whole world and nothing but the world
class World {
	public:
		World (unsigned short world_width, unsigned short world_height);
		
		unsigned short getWidth();
		unsigned short getHeight();
		
	private:
		unsigned short width;     ///< The width of the world.
		unsigned short height;    ///< The height of the world.
};

#endif
