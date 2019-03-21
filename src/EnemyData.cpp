#include "pch.h"
#include "EnemyData.h"

//AI of enemy
EnemyData::EnemyData()
{
	damage = 5;
	AI = AI_WARRIOR;
}

//Enemy move rendering
EnemyData::~EnemyData()
{
}

bool EnemyData::loadTexture()
{
	
	if (!enemy_walking_texture[0].loadFromFile(texturePath1))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	if(texturePath2[0] != '\0')
		if (!enemy_walking_texture[1].loadFromFile(texturePath2))
		{
			printf("Failed to load walking animation texture!\n");
			return false;
		}
	if (!enemyHp_texture.loadFromFile(enemyHpPath))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	//Enemy clips move rendering
	else
	{
		for (int i = 0; i < 20; i++)
		{
			enemy_walk_clips[0][i].x = i * 100;
			enemy_walk_clips[0][i].y = 0;
			enemy_walk_clips[0][i].w = 100;
			enemy_walk_clips[0][i].h = 100;

			enemy_walk_clips[1][i].x = i * 100;
			enemy_walk_clips[1][i].y = 0;
			enemy_walk_clips[1][i].w = 100;
			enemy_walk_clips[1][i].h = 100;

			

		}
		for (int i = 0; i < 3; i++)
		{
			enemyHp_clips[i].x = i * 100;
			enemyHp_clips[i].y = 0;
			enemyHp_clips[i].w = 100;
			enemyHp_clips[i].h = 100;
		}
		//Set sprite clips

		return true;
		
	//Load sprite sheet texture

	}
}