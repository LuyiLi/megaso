#pragma once
#include <SDL.h>
#include "LTexture.h"
class Projectile
{
public:
	Projectile();
	~Projectile();
	static const int Projectile_WIDTH = 10;
	static const int Projectile_HEIGHT = 10;

	//The position of the Projectile
	int posX;
	int posY;
	//the position of the block's boundary of the Projectile 
	int blockPosX;
	int blockPosY;
	bool isExitsting;
	int existTime;
	int damage;
	int time;
	int color;
	SDL_Rect rectArray[16];

	//Maximum axis velocity of the Projectile
	static const int Projectile_VEL = 10;

	//Moves the Projectile and checks collision
	void move();
	bool loadTexture();
	void create(int x, int y, int mousePosX, int mousePosY, int);
	bool checkCollision();
	void updateCollisionBox();

	//Shows the Projectile on the screen
	//The velocity of the Projectile
	double mVelX, mVelY;
	void moveAction(int deltaX, int deltaY);
	//Projectile's collision box
	SDL_Rect mCollider;
	//const int standing_frames = 6;
	SDL_Rect projectile_clips[1];
	LTexture projectile_texture;
	LTexture projectile_texture_blue;
	LTexture projectile_texture_green;
	LTexture projectile_texture_red;
};

