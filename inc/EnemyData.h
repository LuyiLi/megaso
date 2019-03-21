#pragma once
#include <SDL.h>
#include "LTexture.h"

enum EnemyAI
{
	AI_WARRIOR,
	AI_PANGOLIN,
	AI_FLYING,
	AI_THROUGH_WALL
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
	int Enemy_VEL=1;
	int damage;
	char texturePath1[30] = { '\0' };
	char texturePath2[30] = { '\0' };
	char enemyHpPath[30];
	int dropID = 206;
	bool loadTexture();
	int offsetX = 0;
	int offsetY = 0;
	int frame = 0;
	int frameDelay = 0;
	double enemyScale = 0;

	//Shows the Enemy on the screen

	EnemyAI AI;
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	/*����״̬ͼ����Ƭ*/
	//const int walking_frames = 4;
	SDL_Rect enemy_walk_clips[50][50];
	LTexture enemy_walking_texture[50];

	SDL_Rect enemyHp_clips[3];
	LTexture enemyHp_texture;
};

