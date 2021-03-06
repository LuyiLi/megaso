/*
#pragma once
#include "LTexture.h"
#include "droppedItem.h"
#include "Enemy.h"

extern const int g;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Player
{
public:
	//The dimensions of the Player
	static const int Player_WIDTH = 170;
	static const int Player_HEIGHT = 152;
	
	//The position of the Player
	int posX;
	int posY;
	//the position of the block's boundary of the player 
	int blockPosX;
	int blockPosY;

	int healthLimit;
	int healthPoint;

	bool canJump;

	int acceleration;

	SDL_Rect rectArray[16];

	//Maximum axis velocity of the Player
	static const int Player_VEL = 7;

	//Initializes the variables
	Player();

	//Takes key presses and adjusts the Player's velocity
	void handleEvent(SDL_Event& e);

	//Moves the Player and checks collision
	void move();

	void moveAction(int, int);
	bool loadTexture();

	bool checkCollision();
	void pickUpItem(droppedItem *);
	void updateCollisionBox();

	//Shows the Player on the screen
	void render(int camX, int camY);

	int getPosX();
	int getPosY();

	int getVelX();
	int getVelY();

	void getHit(Enemy);
	//The velocity of the Player
	int mVelX, mVelY;

	//Player's collision box
	SDL_Rect mCollider;

	/*վ��״̬ͼ����Ƭ
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	/*����״̬ͼ����Ƭ
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;
};
*/