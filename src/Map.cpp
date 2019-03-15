#include "pch.h"
#include "Map.h"
#include <stdio.h>
#include <io.h>
#include "Entity.h"
#include "Camera.h"
#include "item.h"
#include "pocket.h"
#include <time.h>
#include <stdlib.h>
#include "global.h"

extern Player player;
extern bool intersect(SDL_Rect, SDL_Rect);
extern droppedItem droppedItemList[200];
extern Item itemList[100];
extern pocket mainPocket;
//todo: add enemy collisonbox when breaking

Map::Map()
{
	int mapData[yBlockNumber][xBlockNumber] = {0};
}

bool Map::loadTexture()
{
	if (newMap_texture.loadFromFile("images/newMapTexture.png"))
	{
		for (int i = 0; i < 10; i++)
		{
			newMap_clips[i].x = i*100;
			newMap_clips[i].y = 0;
			newMap_clips[i].w = 100;
			newMap_clips[i].h = 100;
		}
	}

	if (wall_texture.loadFromFile("images/wall.png"))
	{
		for (int i = 0; i < 1; i++)
		{
			wall_clips[i].x = i * 100;
			wall_clips[i].y = 0;
			wall_clips[i].w = 100;
			wall_clips[i].h = 100;
		}
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

void Map::render(int deltaX, int deltaY)
{
	//Do not touch the value below
	int absX = 0, absY = 0;
	int beginX = (player.blockPosX-6) * 50;
	int beginY = (player.blockPosY-4) * 50;
	int endX = (player.blockPosX) * 50;
	int endY = (player.blockPosY) * 50;

	for (int i = player.blockPosY-4; i < player.blockPosY+6; i++)
	{
		for (int j = player.blockPosX-6; j < player.blockPosX+9; j++)
		{

			if (mapData[i][j])
			{
				if (mapData[i][j] <= 100)
				{
					SDL_Rect* currentClip = &newMap_clips[mapData[i][j]];
					newMap_texture.render(beginX + deltaX, beginY + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE, 2);
				}
				else if (mapData[i][j] > 100 && mapData[i][j] <= 200)
				{
					SDL_Rect* currentClip = &wall_clips[mapData[i][j]-100];
					wall_texture.render(beginX + deltaX, beginY + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE, 2);
				}
			}
			beginX += 50;
		}
		beginX = (player.blockPosX-6) * 50;
		beginY += 50;
	}
}
int Map::checkIfExist()
{
	FILE *fp;
	fopen_s(&fp, "map.txt", "r");
	if (fp == NULL)
	{
		return 0;
	}
	else
	{
		fclose(fp);
		return 1;
	}
}

extern int pocketNumber;

void Map::generateMap()
{
	srand(time(NULL));
	generateBiome();
	generateGroundSurface();
	mapWrite();
}
/*
{
	for (int i = 0; i < xBlockNumber; i++)
	{
		for (int j = 0; j < yBlockNumber; j++)
		{
			if (j == 50)
			{
				mapData[j][i] = 1;
			}
			else if (j < 50)
			{
				mapData[j][i] = 0;
			}
			else
			{
				mapData[j][i] = 2;
			}
		}
	}
}
*/
void Map::generateBiome()
{
	int currentBlockNumber = 1;
	int temp;
	int i = 0;
	while (currentBlockNumber < xBlockNumber)
	{
		temp = 200 + 800 * random01();
		groundBiomes[i].biomeRange.x = currentBlockNumber;
		groundBiomes[i].biomeRange.y = 0;
		groundBiomes[i].biomeRange.h = 200;
		groundBiomes[i].biomeRange.w = currentBlockNumber + temp < xBlockNumber ? temp : xBlockNumber - currentBlockNumber;
		switch ((int)(random01() * 6))
		{
		case 0:
			groundBiomes[i].biomeType = GROUND_BIOME_PLAIN;
			break;
		case 1:
			groundBiomes[i].biomeType = GROUND_BIOME_FOREST;
			break;
		case 2:
			groundBiomes[i].biomeType = GROUND_BIOME_DESERT;
			break;
		case 3:
			groundBiomes[i].biomeType = GROUND_BIOME_SNOWLAND;
			break;
		case 4:
			groundBiomes[i].biomeType = GROUND_BIOME_VOCANIC;
			break;
		case 5: 
			groundBiomes[i].biomeType = GROUND_BIOME_MOUNTAIN;
			break;
		}
		currentBlockNumber += temp;
		i++;
	}
	for (; i < 25; i++)
	{
		groundBiomes[i].biomeType = GROUND_BIOME_NULL;
	}
}

void Map::generateGroundSurface()
{
	float a = 0, v = 0, y = 150;
	int x = 1, i = 0;
	while (x < xBlockNumber)
	{	
		if (groundBiomes[i].biomeType == GROUND_BIOME_NULL)
			break;
		else
		{
			while (x >= groundBiomes[i].biomeRange.x && x <= groundBiomes[i].biomeRange.x + groundBiomes[i].biomeRange.w)
			{
				switch (groundBiomes[i].biomeType)
				{
				default:
					break;
				case GROUND_BIOME_PLAIN:
					a = random01() - 0.5;
					if (y > 170)
						v -= 0.1;
					if (y < 120)
						v += 0.1;
					if (v > 0.5)
						v = 0.5;
					if (v < -0.5)
						v = -0.5;
					v = 8 * v / 10 + a;
					y += v;
					break;
				case GROUND_BIOME_DESERT:case GROUND_BIOME_FOREST:case GROUND_BIOME_SNOWLAND:
					a = random01() * 2 - 1;
					if (y > 170)
						v -= 0.5;
					if (y < 80)
						v += 0.5;
					v = 8 * v / 10 + a;
					y += v;
					break;
				case GROUND_BIOME_VOCANIC:case GROUND_BIOME_MOUNTAIN:
					a = random01() * 5 - 2.5;
					if (y > 170)
						v -= 1;
					if (y < 90)
						v += 1;
					v = 8 * v/10 + a;
					y += v;
					break;
				}
				mapData[(int)y][x] = 1;
				x++;
			}
		}
		i++;
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
	for (int i = 0; i < 200; i++)
		if (droppedItemList[i].item.itemType == ITEM_NULL)
		{
			droppedItemList[i].create(50 * x + 15, 50 * y, itemList[mapData[y][x]]);
			break;
		}
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
				mainPocket.pocketData[1][pocketNumber - 1]--;
				return;
			}
	}
}

void Map::mapWrite()
{
	FILE *fp;
	fopen_s(&fp, "map.txt", "w");
	int m = 0, n = 0;
	int num = 0;
	for (m; m < yBlockNumber; m++)
	{
		for (n; n <xBlockNumber; n++)
		{
			num = mapData[m][n];
			fprintf(fp, "%d,", num);
		}
		fprintf(fp,"R,");
		n = 0;
	}
	m = 0;
	fprintf(fp, "E");
	fclose(fp);
}
