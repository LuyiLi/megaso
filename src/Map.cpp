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
	int mapData[yBlockNumber][xBlockNumber] = { 0 };
	int wallData[yBlockNumber][xBlockNumber] = { 0 };
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
		printf("AAAAA");
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
	int beginX = (player.blockPosX-15) * (33);
	int beginY = (player.blockPosY-15) * (33);
	int endX = (player.blockPosX) * (33);
	int endY = (player.blockPosY) * (33);

	for (int i = player.blockPosY-15; i < player.blockPosY+15; i++)
	{
		for (int j = player.blockPosX-15; j < player.blockPosX+16; j++)
		{

			if (mapData[i][j])
			{
				if (mapData[i][j] <= 100)
				{
					SDL_Rect* currentClip = &newMap_clips[mapData[i][j]];
					newMap_texture.render(beginX + deltaX, beginY + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE, 3);
				}
			}
			beginX += 33;
		}
		beginX = (player.blockPosX-15) * (33);
		beginY += (33);
	}
}

void Map::renderWall(int deltaX, int deltaY)
{
	//Do not touch the value below
	int absX = 0, absY = 0;
	int beginX = (player.blockPosX - 15) * (33);
	int beginY = (player.blockPosY - 15) * (33);
	int endX = (player.blockPosX) * (33);
	int endY = (player.blockPosY) * (33);

	for (int i = player.blockPosY - 15; i < player.blockPosY + 15; i++)
	{
		for (int j = player.blockPosX - 15; j < player.blockPosX + 16; j++)
		{
			if (wallData[i][j])
			{
				if (wallData[i][j] > 100 && wallData[i][j] <= 200)
				{
					SDL_Rect* currentClip = &newMap_clips[wallData[i][j] - 101];
					wall_texture.render(beginX + deltaX, beginY + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE, 3);
				}
			}
			beginX += 33;
		}
		beginX = (player.blockPosX - 15) * (33);
		beginY += (33);
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

int Map::checkIfWallExist()
{
	FILE *fp;
	fopen_s(&fp, "wall.txt", "r");
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

void Map::generateWall()
{
	for (int i = 0; i < xBlockNumber; i++)
	{
		for (int j = 0; j < yBlockNumber; j++)
		{
			if (j == 50)
			{
				wallData[j][i] = 101;
			}
			else if (j < 50)
			{
				wallData[j][i] = 0;
			}
			else
			{
				wallData[j][i] = 101;
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

void Map::wallRead()
{
	FILE *fp;
	fopen_s(&fp, "wall.txt", "r");
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
					wallData[m][n] = targetNum;
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
			droppedItemList[i].create((33) * x + 15, (33) * y, itemList[mapData[y][x]]);
			break;
		}
	mapData[y][x] = 0;
	
}

void Map::breakWall(int x, int y)
{
	for (int i = 0; i < 200; i++)
		if (droppedItemList[i].item.itemType == ITEM_NULL)
		{
			droppedItemList[i].create((33) * x + 15, (33) * y, itemList[wallData[y][x]]);
			break;
		}
	wallData[y][x] = 0;

}

void Map::putBlock(int x, int y, int ID)
{
	SDL_Rect tempRect;
	if (!mapData[y][x])
	{
		tempRect.x = (33) * x;
		tempRect.y = (33) * y;
		tempRect.w = (33);
		tempRect.h = (33);
		if(mapData[y][x + 1] || mapData[y + 1][x] || mapData[y - 1][x] || mapData[y][x - 1])
			if (!intersect(tempRect, player.mCollider))
			{
				mapData[y][x] = ID;
				mainPocket.pocketData[1][pocketNumber - 1]--;
				return;
			}
	}
}

void Map::putWall(int x, int y, int ID)
{
	SDL_Rect tempRect;
	if (!wallData[y][x])
	{
		tempRect.x = (33) * x;
		tempRect.y = (33) * y;
		tempRect.w = (33);
		tempRect.h = (33);
		if (wallData[y][x + 1] || wallData[y + 1][x] || wallData[y - 1][x] || wallData[y][x - 1])
			if (!intersect(tempRect, player.mCollider))
			{
				wallData[y][x] = ID;
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

void Map::wallWrite()
{
	FILE *fp;
	fopen_s(&fp, "wall.txt", "w");
	int m = 0, n = 0;
	int num = 0;
	for (m; m < yBlockNumber; m++)
	{
		for (n; n < xBlockNumber; n++)
		{
			num = wallData[m][n];
			fprintf(fp, "%d,", num);
		}
		fprintf(fp, "R,");
		n = 0;
	}
	m = 0;
	fprintf(fp, "E");
	fclose(fp);
}
