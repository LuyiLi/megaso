#include "pch.h"
#include "EnemyData.h"


EnemyData::EnemyData()
{
	damage = 5;
	AI = AI_WARRIOR;
}


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
	if (!enemy_walking_texture[1].loadFromFile(texturePath2))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
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
		//Set sprite clips

		return true;
		
	//Load sprite sheet texture

	}
}