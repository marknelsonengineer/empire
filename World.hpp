#ifndef WORLD_H
#define WORLD_H

class World {
	public:
		World (unsigned short width, unsigned short height);
		
		unsigned short getWidth();
		unsigned short getHeight();
		
	private:
		unsigned short width;
		unsigned short height;
};

#endif
