#include <iostream>

#include "World.hpp"

using namespace std;

int main() {
	cout << "Welcome to Empire\n";
	
	World world(10, 20);
	
	cout << "Width = " << world.getWidth() << endl;
	cout << "Height = " << world.getHeight() << endl;
	
	return 0;
}

