#include "pch.h"
#include "Map.h"
#include <stdio.h>
#include <io.h>
#include "Player.h"
#include "Camera.h"
#include "item.h"

extern Player player;
extern bool intersect(SDL_Rect, SDL_Rect);

Map::Map()
{
	int mapData[xBlockNumber][yBlockNumber] = { 0 };
	
}

bool Map::loadTexture()
{
	if (newMap_texture.loadFromFile("images/newMapTexture.png"))
	{
		newMap_clips[0].x = 0;
		newMap_clips[0].y = 0;
		newMap_clips[0].w = 100;
		newMap_clips[0].h = 100;

		newMap_clips[1].x = 100;
		newMap_clips[1].y = 0;
		newMap_clips[1].w = 100;
		newMap_clips[1].h = 100;

		newMap_clips[2].x = 200;
		newMap_clips[2].y = 0;
		newMap_clips[2].w = 100;
		newMap_clips[2].h = 100;

		newMap_clips[3].x = 300;
		newMap_clips[3].y = 0;
		newMap_clips[3].w = 100;
		newMap_clips[3].h = 100;

		newMap_clips[4].x = 400;
		newMap_clips[4].y = 0;
		newMap_clips[4].w = 100;
		newMap_clips[4].h = 100;

		newMap_clips[5].x = 500;
		newMap_clips[5].y = 0;
		newMap_clips[5].w = 100;
		newMap_clips[5].h = 100;

		newMap_clips[6].x = 600;
		newMap_clips[6].y = 0;
		newMap_clips[6].w = 100;
		newMap_clips[6].h = 100;

		newMap_clips[7].x = 700;
		newMap_clips[7].y = 0;
		newMap_clips[7].w = 100;
		newMap_clips[7].h = 100;

		return true;
	}
	else 
	{
		return false;
	}
	
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
				SDL_Rect* currentClip = &newMap_clips[mapData[i][j]];
				newMap_texture.render(absY + deltaX, absX + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE,2);
			}
			absY += 50;
		}
		absY = 0;
		absX += 50;
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

extern int pocketNumber;

void Map::generateMap()
{
	for (int i = 0; i < xBlockNumber; i++)
	{
		for (int j = 0; j < yBlockNumber; j++)
		{
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
	SDL_Rect tempRect;
	if (!mapData[y][x])
	{
		tempRect.x = 50 * x;
		tempRect.y = 50 * y;
		tempRect.w = 50;
		tempRect.h = 50;
		if(mapData[y][x + 1] || mapData[y + 1][x] || mapData[y - 1][x] || mapData[y][x - 1])
			if (!intersect(tempRect, player.mCollider))
			{
				mapData[y][x] = ID;
				return;
			}
	}
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
