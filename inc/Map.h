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
	GroundBiomeTypes preType;
	GroundBiomeTypes targetType;
	int preAlpha;
	int targetAlpha;
	int scroll[6] = { 0 };
	const int blockSize = 33;
	SDL_Rect newMap_clips[233];
	LTexture newMap_texture;
	SDL_Rect wall_clips[233];
	GroundBiome groundBiomes[25];
	float tempSurfaceArray[5000];
	LTexture wall_texture;
	SDL_Rect bg_clips[1];
	LTexture bg_texture[20][6];
	SDL_Point lightPoint[1600];
	int decrease = 51;
	int lightBlock[40][40];
	static const int xBlockNumber = 5000;
	static const int yBlockNumber = 1000;
	int mapData[yBlockNumber][xBlockNumber];
	int wallData[yBlockNumber][xBlockNumber];
	void drawCircle(int x, int y, int r);
	void render(int deltaX, int deltaY, int blockPosX, int blockPosY);
	bool loadTexture();
	void generateMap();
	int calculateEnemyGenerationRate(int, int);
	void generateGroundSurface();
	void generateRockSurface();
	void generateCave();
	void generateOre();
	void plantTree(int x, int y, GroundBiomeTypes);
	void generateTrees();
	int calculateTreeGenerationRate(GroundBiomeTypes biomeType);
	void breakBlock(int x, int y);
	void putBlock(int x, int y, int ID);
	void mapRead();
	void mapWrite();
	int checkIfExist();
	void updateScroll();
	void renderWall(int deltaX, int deltaY, int blockPosX, int blockPosY);
	void breakWall(int x, int y);
	void putWall(int x, int y, int ID);
	void wallRead();
	void wallWrite();
	void renderBg(GroundBiomeTypes, GroundBiomeTypes);
	int renderBgChange(GroundBiomeTypes tar);
	int checkIfWallExist();
	void calculateLightMatrix(int blockPosX, int blockPosY);
	void calculateLight(int x, int y, int blockPosX, int blockPosY);
	void generateBiome();
	void biomeRead();
	void biomeWrite();
	int checkIfBiomeExist();
	int bgColor[3];
	int frontBgColor[3];
	GroundBiomeTypes currentBiome(int);
	void countBgColor();
	void countFrontBgColor();
	SDL_Rect sun_clips[1];
	LTexture sun_texture;
	SDL_Rect moon_clips[1];
	LTexture moon_texture;
	SDL_Rect star_clips[1];
	LTexture star_texture;
};

