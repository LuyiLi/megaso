#pragma once
#include "SDL.h"

class Map
{
public:
	Map();
	~Map();
	const int blockSize = 100;
	static const int xBlockNumber = 100;
	static const int yBlockNumber = 100;
	int mapData[xBlockNumber][yBlockNumber];
	SDL_Rect collisionRect[5][5];
	int updateCollisionBox();
	void generateMap();
	void mapRead();
	void mapWrite(int targetMap[100][100]);
	int checkIfExist();
};

