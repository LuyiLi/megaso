#pragma once
#include "SDL.h"
#include "LTexture.h"

enum GroundBiomeTypes
{
	GROUND_BIOME_NULL = 0,
	GROUND_BIOME_PLAIN = 1,
	GROUND_BIOME_FOREST = 2,
	GROUND_BIOME_DESERT = 3,
	GROUND_BIOME_SNOWLAND = 4,
	GROUND_BIOME_VOCANIC = 5,
	GROUND_BIOME_MOUNTAIN = 6
};
struct GroundBiome
{
	SDL_Rect biomeRange;
	GroundBiomeTypes biomeType;
};


class Map
{
public:
	Map();
	~Map();
	const int blockSize = 50;
	SDL_Rect newMap_clips[233];
	LTexture newMap_texture;
	SDL_Rect wall_clips[233];
	GroundBiome groundBiomes[25];
	LTexture wall_texture;
	static const int xBlockNumber = 5000;
	static const int yBlockNumber = 1000;
	int mapData[yBlockNumber][xBlockNumber];
	int updateCollisionBox();
	void render(int deltaX, int deltaY);
	bool loadTexture();
	void generateMap();
	void generateBiome();
	void generateGroundSurface();
	void generateRockSurface();
	void breakBlock(int x, int y);
	void putBlock(int x, int y, int ID);
	void mapRead();
	void mapWrite();
	int checkIfExist();
};

