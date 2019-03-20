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
	enemyDataList[1].Enemy_VEL;
	enemyDataList[1].damage;
	strcpy_s(enemyDataList[1].texturePath1, "images/pangolin.png");
	strcpy_s(enemyDataList[1].texturePath2, "images/pangolin_1.png");
	strcpy_s(enemyDataList[1].enemyHpPath, "images/enemyHp.png");
	
}