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
	int Enemy_WIDTH = 170;
	int Enemy_HEIGHT = 152;

	int healthLimit;

	int damage;

	bool loadTexture();

	//Shows the Enemy on the screen

	EnemyAI AI;
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	/*����״̬ͼ����Ƭ*/
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;
};

