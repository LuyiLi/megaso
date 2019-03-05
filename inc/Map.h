#pragma once
#include "SDL.h"

class Map
{
public:
	Map();
	~Map();
	const int blockSize = 100;
	int mapData[100][100];
	SDL_Rect collisionRect[5][5];
	int updateCollisionBox();

	void generateMap();
};

