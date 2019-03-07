#pragma once
#include "SDL.h"
#include "LTexture.h"

class Map
{
public:
	Map();
	~Map();
	const int blockSize = 50;
	SDL_Rect mapClips[2];
	LTexture mapTexture;
	static const int xBlockNumber = 100;
	static const int yBlockNumber = 100;
	int mapData[xBlockNumber][yBlockNumber];
	SDL_Rect collisionRect[5][5];
	int updateCollisionBox();
	void render(int deltaX, int deltaY);
	bool loadTexture();
	void generateMap();
	void breakBlock(int x, int y);
	void putBlock(int x, int y, int ID);
	void mapRead();
	void mapWrite(int targetMap[100][100]);
	int checkIfExist();
};

