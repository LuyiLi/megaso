#include "pch.h"
#include "Map.h"
#include <stdio.h>
#include <io.h>
#include "Entity.h"
#include "Camera.h"
#include "item.h"
#include "pocket.h"

extern Player player;
extern bool intersect(SDL_Rect, SDL_Rect);
extern droppedItem droppedItemList[200];
extern Item itemList[100];
extern pocket mainPocket;
//todo: add enemy collisonbox when breaking

Map::Map()
{
	int mapData[xBlockNumber][yBlockNumber] = {0};
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

			if (mapData[i][j]&& absY + deltaX>-50&& absY + deltaX<SCREEN_WIDTH&&absX + deltaY>-50&& absX + deltaY<SCREEN_HEIGHT)
			{
				if (mapData[i][j] <= 100)
				{
					SDL_Rect* currentClip = &newMap_clips[mapData[i][j]];
					newMap_texture.render(absY + deltaX, absX + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE, 2);
				}
				else if (mapData[i][j] > 100 && mapData[i][j] <= 200)
				{
					SDL_Rect* currentClip = &wall_clips[mapData[i][j]-100];
					wall_texture.render(absY + deltaX, absX + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE, 2);
				}
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
