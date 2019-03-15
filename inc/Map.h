#pragma once
#include "SDL.h"
#include "LTexture.h"


class Map
{
public:
	Map();
	~Map();
	const int blockSize = 50;
	SDL_Rect newMap_clips[233];
	LTexture newMap_texture;
	SDL_Rect wall_clips[233];
	LTexture wall_texture;
	static const int xBlockNumber = 5000;
	static const int yBlockNumber = 1000;
	int mapData[yBlockNumber][xBlockNumber];
	void render(int deltaX, int deltaY);
	bool loadTexture();
	void generateMap();
	void breakBlock(int x, int y);
	void putBlock(int x, int y, int ID);
	void mapRead();
	void mapWrite();
	int checkIfExist();
};

