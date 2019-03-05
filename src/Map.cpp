#include "pch.h"
#include "Map.h"


Map::Map()
{
	int mapData[100][100] = { 0 };
}


Map::~Map()
{
}

int Map::updateCollisionBox()
{
	return 0;
}

void Map::generateMap()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 50; j++)
			mapData[i][j] = 1;
	}
}