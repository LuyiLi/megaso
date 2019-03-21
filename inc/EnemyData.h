#pragma once
#include <SDL.h>
#include "LTexture.h"

enum EnemyAI
{
	AI_WARRIOR,
	AI_PANGOLIN
};

class EnemyData
{
public:
	EnemyData();
	~EnemyData();
	//The dimensions of the Enemy
	int ID = 0;
	int Enemy_WIDTH;
	int Enemy_HEIGHT;
	int healthLimit;
	int Enemy_VEL;
	int damage;
	char texturePath1[30];
	char texturePath2[30];
	char enemyHpPath[30];
	int dropID = 1;
	bool loadTexture();

	//Shows the Enemy on the screen

	EnemyAI AI;
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	//const int walking_frames = 4;
	SDL_Rect enemy_walk_clips[50][50];
	LTexture enemy_walking_texture[50];

	SDL_Rect enemyHp_clips[3];
	LTexture enemyHp_texture;
};

