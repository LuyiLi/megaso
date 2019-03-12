/*
#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "Player.h"

class Enemy
{

public:
	//The dimensions of the Enemy
	int Enemy_WIDTH = 170;
	int Enemy_HEIGHT = 152;

	//The position of the Enemy
	int posX;
	int posY;

	int blockPosX;
	int blockPosY;

	bool canJump;

	int acceleration;

	SDL_Rect rectArray[16];

	//Maximum axis velocity of the Enemy
	int Enemy_VEL = 5;

	//Initializes the variables
	Enemy();

	//Takes key presses and adjusts the Enemy's velocity

	//Moves the Enemy and checks collision
	void move();

	void moveAction(int, int);
	bool loadTexture();

	bool checkCollision();

	void updateCollisionBox();

	//Shows the Enemy on the screen
	void render(int camX, int camY);

	int getPosX();
	int getPosY();

	int getVelX();
	int getVelY();

	//The velocity of the Enemy
	int mVelX, mVelY;

	//Enemy's collision box
	SDL_Rect mCollider;

	/*Õ¾Á¢×´Ì¬Í¼ÏñÇÐÆ¬
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	/*ÐÐ×ß×´Ì¬Í¼ÏñÇÐÆ¬
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;
};
*/
