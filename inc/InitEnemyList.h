#pragma once
#include "EnemyData.h"
#include <string.h>

extern EnemyData enemyDataList[10];

void initEnemyDataList()
{
	enemyDataList[1].ID = 1;
	enemyDataList[1].Enemy_WIDTH = 170;
	enemyDataList[1].Enemy_HEIGHT = 152;
	enemyDataList[1].healthLimit = 100;
	enemyDataList[1].Enemy_VEL=1;
	enemyDataList[1].damage=10;
	enemyDataList[1].AI = AI_WARRIOR;
	enemyDataList[1].dropChance = 1; //todo
	enemyDataList[1].dropID = 206; //todo
	strcpy_s(enemyDataList[1].texturePath1, "images/pangolin.png");
	strcpy_s(enemyDataList[1].texturePath2, "images/pangolin_1.png");
	strcpy_s(enemyDataList[1].enemyHpPath, "images/enemyHp.png");

	enemyDataList[2].ID = 2;
	enemyDataList[2].Enemy_WIDTH = 50;
	enemyDataList[2].Enemy_HEIGHT = 50;
	enemyDataList[2].healthLimit = 100;
	enemyDataList[2].Enemy_VEL = 2;
	enemyDataList[2].damage = 15;
	enemyDataList[2].AI = AI_WARRIOR;
	enemyDataList[2].offsetX = 0;
	enemyDataList[2].offsetY = 5;
	enemyDataList[2].frame = 2;
	enemyDataList[2].frameDelay = 10;
	enemyDataList[2].enemyScale = 1;
	strcpy_s(enemyDataList[2].texturePath1, "images/stump.png");
	strcpy_s(enemyDataList[2].enemyHpPath, "images/enemyHp.png");

	enemyDataList[3].ID = 3;
	enemyDataList[3].Enemy_WIDTH = 50;
	enemyDataList[3].Enemy_HEIGHT = 50;
	enemyDataList[3].healthLimit = 100;
	enemyDataList[3].Enemy_VEL = 5;
	enemyDataList[3].damage=10;
	enemyDataList[3].AI = AI_THROUGH_WALL;
	enemyDataList[3].offsetX = 0;
	enemyDataList[3].offsetY = 0;
	enemyDataList[3].frame = 4;
	enemyDataList[3].frameDelay = 5;
	enemyDataList[3].enemyScale = 1;
	strcpy_s(enemyDataList[3].texturePath1, "images/crystal.png");
	strcpy_s(enemyDataList[3].enemyHpPath, "images/enemyHp.png");

	enemyDataList[4].ID = 4;
	enemyDataList[4].Enemy_WIDTH = 120;
	enemyDataList[4].Enemy_HEIGHT = 20;
	enemyDataList[4].healthLimit = 100;
	enemyDataList[4].Enemy_VEL = 10;
	enemyDataList[4].damage = 20;
	enemyDataList[4].AI = AI_FLYING;
	enemyDataList[4].offsetX = 0;
	enemyDataList[4].offsetY = 0;
	enemyDataList[4].frame = 8;
	enemyDataList[4].frameDelay = 5;
	enemyDataList[4].enemyScale = 0.75;
	strcpy_s(enemyDataList[4].texturePath1, "images/dragon.png");
	strcpy_s(enemyDataList[4].enemyHpPath, "images/enemyHp.png");

	enemyDataList[5].ID = 5;
	enemyDataList[5].Enemy_WIDTH = 70;
	enemyDataList[5].Enemy_HEIGHT = 90;
	enemyDataList[5].healthLimit = 100;
	enemyDataList[5].Enemy_VEL = 2;
	enemyDataList[5].damage = 20;
	enemyDataList[5].AI = AI_WARRIOR;
	enemyDataList[5].offsetX = -10;
	enemyDataList[5].offsetY = 36;
	enemyDataList[5].frame = 4;
	enemyDataList[5].frameDelay = 5;
	enemyDataList[5].enemyScale = 1;
	strcpy_s(enemyDataList[5].texturePath1, "images/snowman.png");
	strcpy_s(enemyDataList[5].enemyHpPath, "images/enemyHp.png");

	enemyDataList[6].ID = 6;
	enemyDataList[6].Enemy_WIDTH = 50;
	enemyDataList[6].Enemy_HEIGHT = 90;
	enemyDataList[6].healthLimit = 100;
	enemyDataList[6].Enemy_VEL = 2;
	enemyDataList[6].damage = 15;
	enemyDataList[6].AI = AI_WARRIOR;
	enemyDataList[6].offsetX = -15;
	enemyDataList[6].offsetY = 30;
	enemyDataList[6].frame = 4;
	enemyDataList[6].frameDelay = 5;
	enemyDataList[6].enemyScale = 1;
	strcpy_s(enemyDataList[6].texturePath1, "images/cactus.png");
	strcpy_s(enemyDataList[6].enemyHpPath, "images/enemyHp.png");

	enemyDataList[7].ID = 7;
	enemyDataList[7].Enemy_WIDTH = 80;
	enemyDataList[7].Enemy_HEIGHT = 80;
	enemyDataList[7].healthLimit = 100;
	enemyDataList[7].Enemy_VEL = 3;
	enemyDataList[7].damage = 15;
	enemyDataList[7].AI = AI_JUMP_WARRIOR;
	enemyDataList[7].offsetX = 0;
	enemyDataList[7].offsetY = 20;
	enemyDataList[7].frame = 2;
	enemyDataList[7].frameDelay = 5;
	enemyDataList[7].enemyScale = 1;
	strcpy_s(enemyDataList[7].texturePath1, "images/yellow.png");
	strcpy_s(enemyDataList[7].enemyHpPath, "images/enemyHp.png");
}