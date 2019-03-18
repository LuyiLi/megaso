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
int preBgAlpha = 255;
int tarBgAlpha = 0;
//todo: add enemy collisonbox when breaking

Map::Map()
{
	int scroll[6] = { 0 };
	int mapData[yBlockNumber][xBlockNumber] = { 0 };
	int wallData[yBlockNumber][xBlockNumber] = { 0 };
}

bool Map::loadTexture()
{
	//create the texture of new map
	if (newMap_texture.loadFromFile("images/newMapTexture.png"))
	{
		for (int i = 0; i < 50; i++)
		{
			newMap_clips[i].x = i*100;
			newMap_clips[i].y = 0;
			newMap_clips[i].w = 100;
			newMap_clips[i].h = 100;
		}
	}

	if (wall_texture.loadFromFile("images/wall.png"))
	{
		for (int i = 0; i < 6; i++)
		{
			wall_clips[i].x = i * 100;
			wall_clips[i].y = 0;
			wall_clips[i].w = 100;
			wall_clips[i].h = 100;
		}
	}

	if (bg_texture[GROUND_BIOME_PLAIN][5].loadFromFile("images/plain1.png") && bg_texture[GROUND_BIOME_PLAIN][4].loadFromFile("images/plain2.png") && bg_texture[GROUND_BIOME_PLAIN][3].loadFromFile("images/plain3.png")
		&& bg_texture[GROUND_BIOME_PLAIN][2].loadFromFile("images/plain4.png") && bg_texture[GROUND_BIOME_PLAIN][1].loadFromFile("images/plain5.png") && bg_texture[GROUND_BIOME_PLAIN][0].loadFromFile("images/plain6.png")
		&& bg_texture[GROUND_BIOME_FOREST][5].loadFromFile("images/forest1.png") && bg_texture[GROUND_BIOME_FOREST][4].loadFromFile("images/forest2.png") && bg_texture[GROUND_BIOME_FOREST][3].loadFromFile("images/forest3.png")
		&& bg_texture[GROUND_BIOME_FOREST][2].loadFromFile("images/forest4.png") && bg_texture[GROUND_BIOME_FOREST][1].loadFromFile("images/forest5.png") && bg_texture[GROUND_BIOME_FOREST][0].loadFromFile("images/forest6.png")
		&& bg_texture[GROUND_BIOME_DESERT][5].loadFromFile("images/desert1.png") && bg_texture[GROUND_BIOME_DESERT][4].loadFromFile("images/desert2.png") && bg_texture[GROUND_BIOME_DESERT][3].loadFromFile("images/desert3.png")
		&& bg_texture[GROUND_BIOME_DESERT][2].loadFromFile("images/desert4.png") && bg_texture[GROUND_BIOME_DESERT][1].loadFromFile("images/desert5.png") && bg_texture[GROUND_BIOME_DESERT][0].loadFromFile("images/desert6.png")
		&& bg_texture[GROUND_BIOME_MOUNTAIN][5].loadFromFile("images/mountain1.png") && bg_texture[GROUND_BIOME_MOUNTAIN][4].loadFromFile("images/mountain2.png") && bg_texture[GROUND_BIOME_MOUNTAIN][3].loadFromFile("images/mountain3.png")
		&& bg_texture[GROUND_BIOME_MOUNTAIN][2].loadFromFile("images/mountain4.png") && bg_texture[GROUND_BIOME_MOUNTAIN][1].loadFromFile("images/mountain5.png") && bg_texture[GROUND_BIOME_MOUNTAIN][0].loadFromFile("images/mountain6.png")
		&& bg_texture[GROUND_BIOME_SNOWLAND][5].loadFromFile("images/snow1.png") && bg_texture[GROUND_BIOME_SNOWLAND][4].loadFromFile("images/snow2.png") && bg_texture[GROUND_BIOME_SNOWLAND][3].loadFromFile("images/snow3.png")
		&& bg_texture[GROUND_BIOME_SNOWLAND][2].loadFromFile("images/snow4.png") && bg_texture[GROUND_BIOME_SNOWLAND][1].loadFromFile("images/snow5.png") && bg_texture[GROUND_BIOME_SNOWLAND][0].loadFromFile("images/snow6.png")
		&& bg_texture[GROUND_BIOME_VOCANIC][5].loadFromFile("images/Volcanic1.png") && bg_texture[GROUND_BIOME_VOCANIC][4].loadFromFile("images/Volcanic2.png") && bg_texture[GROUND_BIOME_VOCANIC][3].loadFromFile("images/Volcanic3.png")
		&& bg_texture[GROUND_BIOME_VOCANIC][2].loadFromFile("images/Volcanic4.png") && bg_texture[GROUND_BIOME_VOCANIC][1].loadFromFile("images/Volcanic5.png") && bg_texture[GROUND_BIOME_VOCANIC][0].loadFromFile("images/Volcanic6.png"))
	{
		bg_clips[0].x = 0;
		bg_clips[0].y = 0;
		bg_clips[0].w = 1800;
		bg_clips[0].h = 1200;
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
	int beginX = (player.blockPosX-20) * (33);
	int beginY = (player.blockPosY-20) * (33);
	int endX = (player.blockPosX) * (33);
	int endY = (player.blockPosY) * (33);
	for (int i = player.blockPosY-20; i < player.blockPosY+20; i++)
	{
		for (int j = player.blockPosX-20; j < player.blockPosX+20; j++)
		{
			if (mapData[i][j])
			{
				if (mapData[i][j] <= 100)
				{
					SDL_Rect* currentClip = &newMap_clips[mapData[i][j]];
					newMap_texture.setColor(lightBlock[j- player.blockPosX + 20][i - player.blockPosY + 20], lightBlock[j - player.blockPosX + 20][i - player.blockPosY + 20], lightBlock[j - player.blockPosX + 20][i - player.blockPosY + 20]);
					newMap_texture.render(beginX + deltaX, beginY + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE, 3);
				}
			}
			beginX += 33;
		}
		beginX = (player.blockPosX-20) * (33);
		beginY += (33);
	}
	
}

void Map::calculateLight(int x, int y)
{
	int lightValue = lightBlock[x][y] / 1.3;
	int diagonalLightValue = lightBlock[x][y] / 1.5;
	int airLightValue = lightBlock[x][y] / 1.1;
	int diagonalAirLightValue = lightBlock[x][y] / 1.15;

	if (lightBlock[x][y] <= 1) {
		return;
	}

	if (x - 1 < 0)
		return;

	if (lightValue > lightBlock[x - 1][y]) {
		lightBlock[x - 1][y] = mapData[y + player.blockPosY - 20][x + player.blockPosX - 21] ? lightValue : airLightValue;
		calculateLight(x - 1, y);
	}

	if (y - 1 < 0)
			return;
	if (lightValue > lightBlock[x][y - 1]) {
		lightBlock[x][y - 1] = mapData[y + player.blockPosY - 21][x + player.blockPosX - 20] ? lightValue : airLightValue;
		calculateLight(x, y - 1);
	}

	if (x + 1 > 40)
		return;
	if (lightValue > lightBlock[x + 1][y]) {
		lightBlock[x + 1][y] = mapData[y + player.blockPosY - 20][x + player.blockPosX - 19] ? lightValue : airLightValue;
		calculateLight(x + 1, y);
	}

	if (y + 1 > 40)
		return;
	if (lightValue > lightBlock[x][y + 1]) {
		lightBlock[x][y + 1] = mapData[y + player.blockPosY - 19][x + player.blockPosX - 20] ? lightValue : airLightValue;;
		calculateLight(x, y + 1);
	}

	if (diagonalLightValue > lightBlock[x + 1][y + 1]) {
		lightBlock[x + 1][y + 1] = mapData[y + player.blockPosY - 19][x + player.blockPosX - 19] ? diagonalLightValue : diagonalAirLightValue;
		calculateLight(x + 1, y + 1);
	}

	if (diagonalLightValue > lightBlock[x - 1][y + 1]) {
		lightBlock[x - 1][y + 1] = mapData[y + player.blockPosY - 19][x + player.blockPosX - 21] ? diagonalLightValue : diagonalAirLightValue;
		calculateLight(x - 1, y + 1);
	}

	if (diagonalLightValue > lightBlock[x + 1][y - 1]) {
		lightBlock[x + 1][y - 1] = mapData[y + player.blockPosY - 21][x + player.blockPosX - 19] ? diagonalLightValue : diagonalAirLightValue;
		calculateLight(x + 1, y - 1);
	}

	if (diagonalLightValue > lightBlock[x - 1][y - 1]) {
		lightBlock[x - 1][y - 1] = mapData[y + player.blockPosY - 21][x + player.blockPosX - 21] ? diagonalLightValue : diagonalAirLightValue;
		calculateLight(x - 1, y - 1);
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
	int num = 0;
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			lightBlock[i][j] = 0;
		}
	}
	for (int i = player.blockPosX - 20; i < player.blockPosX + 20; i++)
	{
		for (int j = player.blockPosY - 20; j < player.blockPosY + 20; j++)
		{
			if (!mapData[j][i] && !wallData[j][i])
			{
				if (mapData[j + 1][i] || mapData[j - 1][i] || mapData[j][i + 1] || mapData[j][i - 1]|| wallData[j + 1][i] || wallData[j - 1][i] || wallData[j][i + 1] || wallData[j][i - 1])
				{
					lightPoint[num].x = i - player.blockPosX + 20;
					lightPoint[num].y = j - 1 - player.blockPosY + 20 + 1;
					lightBlock[lightPoint[num].x][lightPoint[num].y] = 255;
					calculateLight(lightPoint[num].x, lightPoint[num].y);
					num++;
				}
				
			}
		}
	}
	//lightBlock[20][20] = 255;
	//lightBlock[21][20] = 255;
	lightBlock[21][21] = 255;
	//lightBlock[20][21] = 255;
	//calculateLight(20, 20);
	//calculateLight(21, 20);
	//calculateLight(20, 21);
	calculateLight(21, 21);
	num = 0;
	for (int i = player.blockPosY - 15; i < player.blockPosY + 15; i++)
	{
		for (int j = player.blockPosX - 15; j < player.blockPosX + 16; j++)
		{
			if (wallData[i][j])
			{
				if (wallData[i][j] > 100 && wallData[i][j] <= 200)
				{
					wall_texture.setColor(lightBlock[j - player.blockPosX + 20][i - player.blockPosY + 20], lightBlock[j - player.blockPosX + 20][i - player.blockPosY + 20], lightBlock[j - player.blockPosX + 20][i - player.blockPosY + 20]);
					SDL_Rect* currentClip = &newMap_clips[wallData[i][j] - 100];
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

int Map::checkIfBiomeExist()
{
	FILE *fp;
	fopen_s(&fp, "biome.txt", "r");
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

void Map::drawCircle(int x, int y, int r)
{
	if (x - r <= 0 || x + r >= xBlockNumber || y - r <= 0 || y + r >= yBlockNumber)
		return;
	for(int i = -r; i <= r; i++)
		for (int j = -r; j <= r; j++)
		{
			if (i*i + j * j <= r*r && i != r && j != r )
				mapData[y + j][x + i] = 0;
		}
}

void Map::generateMap()
{
	srand(time(NULL));
	generateBiome();
	generateGroundSurface();
	generateRockSurface();
	generateOre();
	generateCave();
	generateTrees();
}

void Map::generateBiome()
{
	int currentBlockNumber = 1;
	int temp;
	int i = 0;
	while (currentBlockNumber < xBlockNumber)
	{
		temp = 200 + 400 * random01();
		groundBiomes[i].biomeRange.x = currentBlockNumber;
		groundBiomes[i].biomeRange.y = 0;
		groundBiomes[i].biomeRange.h = 200;
		groundBiomes[i].biomeRange.w = currentBlockNumber + temp < xBlockNumber ? temp : xBlockNumber - currentBlockNumber;
		switch ((int)(random01() * 14))
		{
		case 0:case 1:case 10:case 11:case 12:
			groundBiomes[i].biomeType = GROUND_BIOME_PLAIN;
			break;
		case 2:case 3:case 13:
			groundBiomes[i].biomeType = GROUND_BIOME_FOREST;
			break;
		case 4:case 5:
			groundBiomes[i].biomeType = GROUND_BIOME_DESERT;
			break;
		case 6: case 7:
			groundBiomes[i].biomeType = GROUND_BIOME_SNOWLAND;
			break;
		case 8:
			groundBiomes[i].biomeType = GROUND_BIOME_VOCANIC;
			break;
		case 9: 
			groundBiomes[i].biomeType = GROUND_BIOME_MOUNTAIN;
			break;
		}
		if (currentBlockNumber < 2500 && currentBlockNumber + temp >2500)
			groundBiomes[i].biomeType = GROUND_BIOME_PLAIN;
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
					v = 4 * v / 8 + a;
					y += v;
					break;
				case GROUND_BIOME_DESERT:case GROUND_BIOME_FOREST:case GROUND_BIOME_SNOWLAND:
					a = random01() - 0.5;
					if (y > 170)
						v -= 0.5;
					if (y < 80)
						v += 0.5;
					v = 8 * v / 10 + a;
					y += v;
					break;
				case GROUND_BIOME_VOCANIC:case GROUND_BIOME_MOUNTAIN:
					a = random01() * 3 - 1.5;
					if (y > 170)
						v -= 0.5;
					if (y < 70)
						v += 0.5;
					v = 9 * v/10 + a;
					y += v;
					break;
				}
				mapData[(int)y][x] = 1;
				wallData[(int)y][x] = 101;
				//flatten the ground
				if (mapData[(int)y][x - 3])
				{
					mapData[(int)y][x - 2] = 1;
					mapData[(int)y + 1][x - 2] = 0;
					mapData[(int)y + 2][x - 2] = 0;
					mapData[(int)y - 1][x - 2] = 0;
					mapData[(int)y - 2][x - 2] = 0;
					wallData[(int)y][x - 2] = 101;
					wallData[(int)y + 1][x - 2] = 0;
					wallData[(int)y + 2][x - 2] = 0;
					wallData[(int)y - 1][x - 2] = 0;
					wallData[(int)y - 2][x - 2] = 0;
					tempSurfaceArray[x - 2] = y;
				}
				if (mapData[(int)y][x - 2])
				{
					mapData[(int)y][x - 1] = 1;
					mapData[(int)y + 1][x - 1] = 0;
					mapData[(int)y + 2][x - 1] = 0;
					mapData[(int)y - 1][x - 1] = 0;
					mapData[(int)y - 2][x - 1] = 0;
					wallData[(int)y][x - 1] = 101;
					wallData[(int)y + 1][x - 1] = 0;
					wallData[(int)y + 2][x - 1] = 0;
					wallData[(int)y - 1][x - 1] = 0;
					wallData[(int)y - 2][x - 1] = 0;
					tempSurfaceArray[x - 1] = y;
				}
				tempSurfaceArray[x] = y;
				x++;
			}
		}
		i++;
	}
}

void Map::generateRockSurface()
{
	float temp;
	int tempY;
	for (int i = 0; i < xBlockNumber; i++)
	{
		temp = 0;
		for (int j = i-4; j < i+4; j++)
		{
			temp += j < 0 || j > xBlockNumber ? 150 : tempSurfaceArray[j];
		}
		tempY = (int)(temp / 9 + 40);
		for (int k = tempY; !mapData[k][i]; k--)
		{
			mapData[k][i] = 2;
			wallData[k][i] = 101;
		}
			
		for (int k = tempY + 1; k < yBlockNumber; k++)
		{
			mapData[k][i] = 7;
			wallData[k][i] = 103;
		}
			
	}
}

void Map::generateOre()
{
	int x, y;
	for (int i = 0; i < 6; i ++)
		for (int j = 0; j < 2000; j++)
		{
			x = random01() * xBlockNumber;
			y = random01() * yBlockNumber;
			if (x < 30 || x > xBlockNumber - 30 || y < 30 || y > yBlockNumber - 30)
				continue;
			if (mapData[y][x] == 7)
			{
				for (int k = - 5; k <= 5; k ++)
					for (int l = -5; l <= 5; l++)
					{
						if (mapData[k + y][l + x] == 7)
							if (random01()*(abs(k) + abs(l)) < 2)
								mapData[k + y][l + x] = i + 8;
					}
			}
		}
}

void Map::generateCave()
{
	for (int i = 0; i < 50; i++)
	{
		int x = 200 + random01() * (xBlockNumber - 400);
		int y = 200 + random01() * (yBlockNumber - 300);
		float ax, ay, vx = 0, vy = 0;
		int rBase = random01() * 2 + 2;
		for (int j = 0; j < 300; j++)
		{
			ax = random01() - 0.5;
			ay = random01() - 0.5;
			vx = ax + 0.99*vx;
			vy = ay + 0.99*vy;
			if (y < 100)
				vy += 0.5;
			if (y > yBlockNumber - 100)
				vy -= 0.5;
			y += vy;
			if (abs(vx) > 2)
				x += vx;
			else
			{
				if (vx > 0)
					x += 2;
				else
					x -= 2;
			}
			drawCircle(x, y, rBase + random01() * 2 + abs(vx)/4 + abs(vy)/4);
		}
	}
}

void Map::generateTrees()
{
	int currentBiomeNum = 0;
	int generationRate = calculateTreeGenerationRate(groundBiomes[0].biomeType);
	for (int i = 30; i < xBlockNumber - 30; i++)
	{
		if (i > groundBiomes[currentBiomeNum].biomeRange.x + groundBiomes[currentBiomeNum].biomeRange.w)
			currentBiomeNum++;
		if (10 * random01() < generationRate)
			plantTree(i, (int)tempSurfaceArray[i], groundBiomes[currentBiomeNum].biomeType);
	}
}

int Map::calculateTreeGenerationRate(GroundBiomeTypes biomeType)
{
	switch (biomeType)
	{
	case GROUND_BIOME_PLAIN:case GROUND_BIOME_SNOWLAND:
		return 1;
		break;
	case GROUND_BIOME_DESERT:case GROUND_BIOME_VOCANIC:
		return 0;
		break;
	case GROUND_BIOME_MOUNTAIN:
		return 3;
		break;
	case GROUND_BIOME_FOREST:
		return 6;
		break;
	default:
		return 0;
		break;
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

		//avoid having a negative number in the text
		int negativeFlag = 0;
		while ((presentChar = fgetc(fp)) != EOF)
		{
			//'E'is the symbol of the end of file
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
					//create a new line
					if (presentChar == 'R')
					{
						fgetc(fp);
						n = 0;
						m++;
					}
				}
				//create a space between two numbers
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

void Map::biomeRead()
{
	FILE *fp;
	fopen_s(&fp, "biome.txt", "r");
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
					switch (n)
					{
					case 0:
						groundBiomes[m].biomeRange.x = targetNum; break;
					case 1:
						groundBiomes[m].biomeRange.y = targetNum; break;
					case 2:
						groundBiomes[m].biomeRange.w = targetNum; break;
					case 3:
						groundBiomes[m].biomeRange.h = targetNum; break;
					case 4:
						switch (targetNum)
						{
						case 1:
							groundBiomes[m].biomeType = GROUND_BIOME_PLAIN;
							break;
						case 2:
							groundBiomes[m].biomeType = GROUND_BIOME_FOREST;
							break;
						case 3:
							groundBiomes[m].biomeType = GROUND_BIOME_DESERT;
							break;
						case 4:
							groundBiomes[m].biomeType = GROUND_BIOME_SNOWLAND;
							break;
						case 5:
							groundBiomes[m].biomeType = GROUND_BIOME_VOCANIC;
							break;
						case 6:
							groundBiomes[m].biomeType = GROUND_BIOME_MOUNTAIN;
							break;
						default:
							break;
						}
					default:
						break;
					}
					n++;
					targetNum = 0;
				}
			}
		}
		fclose(fp);
	}
}

void Map::plantTree(int x, int y, GroundBiomeTypes)
{
	int leafID = 105, trunkID = 104;
	int treeHeight = random01() * 6 + 7;
	if (y < 30 || mapData[y][x] != 1)
		return;
	//check if it is possible to plant trees
	for (int i = x - 3; i <= x + 3; i++)
		for (int j = y - treeHeight + 6; j > y - treeHeight - 4; j--)
			if (wallData[j][i])
				return;
	switch ((int)(random01() * 2))
	{
	case 0:
		wallData[y - treeHeight - 3][x] = leafID;
		for (int i = x - 1; i <= x+1; i++)
			wallData[y - treeHeight - 2][i] = leafID;
		for (int i = x - 2; i <= x + 2; i++)
			wallData[y - treeHeight - 1][i] = leafID;
		for (int i = x - 2; i <= x + 2; i++)
			wallData[y - treeHeight][i] = leafID;
		for (int i = x - 1; i <= x + 1; i++)
			wallData[y - treeHeight + 1][i] = leafID;
		for (int i = 1; i <= treeHeight; i++)
			wallData[y - i][x] = trunkID;
		break;
	case 1:
		wallData[y - treeHeight - 2][x] = leafID;
		for (int i = x - 1; i <= x + 1; i++)
			wallData[y - treeHeight - 1][i] = leafID;
		wallData[y - treeHeight + 1][x - 1] = leafID;
		wallData[y - treeHeight + 2][x - 1] = leafID;
		wallData[y - treeHeight + 3][x + 1] = leafID;
		wallData[y - treeHeight + 4][x + 1] = leafID;
		for (int i = 1; i <= treeHeight; i++)
			wallData[y - i][x] = trunkID;
		break;
	}
	mapData[y][x] = 2;
}

void Map::breakBlock(int x, int y)
{
	if (abs(x - player.blockPosX) + abs(y - player.blockPosY) <= 4 && (!mapData[y+1][x] || !mapData[y][x+1] || !mapData[y-1][x] || !mapData[y][x-1]))
	{
		for (int i = 0; i < 200; i++)
			if (droppedItemList[i].item.itemType == ITEM_NULL)
			{
				droppedItemList[i].create((33) * x + 15, (33) * y, itemList[mapData[y][x]]);
				break;
			}
		mapData[y][x] = 0;
	}
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
//put block into the map, reduce one from your pocket
void Map::putBlock(int x, int y, int ID)
{
	SDL_Rect tempRect;
	if (!mapData[y][x]&& abs(x - player.blockPosX) + abs(y - player.blockPosY) <= 6)
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

void Map::biomeWrite()
{
	FILE *fp;
	fopen_s(&fp, "biome.txt", "w");
	int m = 0, n = 0;
	int num = 0;
	for (m; m < 25; m++)
	{
		for (n; n < 5; n++)
		{
			switch (n)
			{
			case 0:
				fprintf(fp, "%d,", groundBiomes[m].biomeRange.x); break;
			case 1:
				fprintf(fp, "%d,", groundBiomes[m].biomeRange.y); break;
			case 2:
				fprintf(fp, "%d,", groundBiomes[m].biomeRange.w); break;
			case 3:
				fprintf(fp, "%d,", groundBiomes[m].biomeRange.h); break;
			case 4:
				fprintf(fp, "%d,", (int)groundBiomes[m].biomeType); break;
			default:
				break;
			}
		}
		fprintf(fp, "R,");
		n = 0;
	}
	m = 0;
	fprintf(fp, "E");
	fclose(fp);
}

void Map::renderBg(GroundBiomeTypes pre, GroundBiomeTypes tar)
{
	scroll[0] -= player.mVelX / 4;
	scroll[1] -= player.mVelX / 3.6;
	scroll[2] -= player.mVelX / 3.3;
	scroll[3] -= player.mVelX / 3;
	scroll[4] -= player.mVelX / 2.8;
	scroll[5] -= player.mVelX / 2.5;
	for (int i = 0; i < 6; i++)
	{
		if (abs(scroll[i]) > 900)
		{
			scroll[i] = 0;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		bg_texture[pre][i].setAlpha(preBgAlpha);
		bg_texture[pre][i].render(scroll[i], 0, bg_clips, 0, NULL, SDL_FLIP_NONE, 2);
		bg_texture[pre][i].render(scroll[i] + 900, 0, bg_clips, 0, NULL, SDL_FLIP_NONE, 2);
		bg_texture[pre][i].render(scroll[i] - 900, 0, bg_clips, 0, NULL, SDL_FLIP_NONE, 2);

		bg_texture[tar][i].setAlpha(tarBgAlpha);
		bg_texture[tar][i].render(scroll[i], 0, bg_clips, 0, NULL, SDL_FLIP_NONE, 2);
		bg_texture[tar][i].render(scroll[i] + 900, 0, bg_clips, 0, NULL, SDL_FLIP_NONE, 2);
		bg_texture[tar][i].render(scroll[i] - 900, 0, bg_clips, 0, NULL, SDL_FLIP_NONE, 2);
	}
}

int Map::renderBgChange(GroundBiomeTypes tar)
{
	for (int i = 0; i < 6; i++)
	{
		if (abs(scroll[i]) > 900)
		{
			scroll[i] = 0;
		}
	}
	tarBgAlpha += 3;
	preBgAlpha -= 3;
	if (tarBgAlpha >= 250)
	{

		preBgAlpha = 255;
		tarBgAlpha = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

GroundBiomeTypes Map::currentBiome(int presentPosX)
{
	for (int i = 0; i < 25; i++)
	{
		if (presentPosX >= groundBiomes[i].biomeRange.x && presentPosX < groundBiomes[i].biomeRange.w+ groundBiomes[i].biomeRange.x)
		{
			return groundBiomes[i].biomeType;
		}
	}
	return GROUND_BIOME_PLAIN;
}
