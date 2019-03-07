#include "pch.h"
#include "Map.h"
#include <stdio.h>
#include <io.h>
#include "Player.h"
#include "Camera.h"
extern Player player;

Map::Map()
{
	int mapData[xBlockNumber][yBlockNumber] = { 0 };
	
}

bool Map::loadTexture()
{
	if (mapTexture.loadFromFile("images/mapTexture.png"))
	{
		mapClips[0].x = 0;
		mapClips[0].y = 0;
		mapClips[0].w = 200;
		mapClips[0].h = 200;

		mapClips[1].x = 200;
		mapClips[1].y = 0;
		mapClips[1].w = 200;
		mapClips[1].h = 200;
		return true;
	}
	return false;
}

Map::~Map()
{
}

int Map::updateCollisionBox()
{
	return 0;
}

void Map::render(int deltaX, int deltaY)
{
	int absX = 0, absY = 0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{

			if (mapData[i][j])
			{
				SDL_Rect* currentClip = &mapClips[mapData[i][j] - 1];
				mapTexture.render(absY + deltaX, absX + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE);
			}
			absY += 100;
		}
		absY = 0;
		absX += 100;
	}
}
int Map::checkIfExist()
{
	FILE *fp;
	fopen_s(&fp, "map.txt", "r");
	if (fp == NULL)
	{
		return 1;
	}
	else
	{
		fclose(fp);
		return 0;
	}
}

void Map::generateMap()
{
	for (int i = 0; i < xBlockNumber; i++)
	{
		for (int j = 50; j <yBlockNumber; j++)
			if (j == 50)
			{
				mapData[j][i] = 1;
			}
			else 
			{
				mapData[j][i] = 2;
			}
	}
}

void Map::mapRead()
{
	FILE *fp;
	fopen_s(&fp, "map.txt", "r");
	char presentChar;
	int targetNum = 0;
	int num = 0;
	if (fp != NULL)
	{
		int m = 0, n = 0;
		int negativeFlag = 0;
		while ((presentChar = fgetc(fp)) != EOF)
		{
			if (presentChar == 'E')
			{
				fclose(fp);
				return;
			}
			else
			{
				if (presentChar != ',')
				{

					if (presentChar == '-')
					{
						negativeFlag = 1;
					}
					if ((int)presentChar >= 48 && (int)presentChar <= 57)
					{
						targetNum = targetNum * 10 + (int)presentChar - 48;
					}
					if (presentChar == 'R')
					{
						fgetc(fp);
						n = 0;
						m++;
					}
				}
				else if (presentChar == ',')
				{
					if (negativeFlag)
					{
						targetNum = targetNum * (-1);
					}
					negativeFlag = 0;
					mapData[m][n] = targetNum;
					n++;
					targetNum = 0;
				}
			}
		}
		fclose(fp);
	}
}

/*
@brief break a block on the map
@param The BLOCK position of the map
*/

void Map::breakBlock(int x, int y)
{
	mapData[y][x] = 0;
}

void Map::putBlock(int x, int y, int ID)
{
	mapData[y][x] = ID;
}

void Map::mapWrite(int targetMap[100][100])
{
	FILE *fp;
	fopen_s(&fp, "map.txt", "w");
	int m = 0, n = 0;
	int num = 0;
	for (m; m < 100; m++)
	{
		for (n; n <100; n++)
		{
			num = targetMap[m][n];
			fprintf(fp, "%d,", num);
		}
		fprintf(fp,"R,");
		n = 0;
	}
	m = 0;
	fprintf(fp, "E");
	fclose(fp);
}
