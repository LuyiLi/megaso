#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "droppedItem.h"

class Enemy;
class Player;


extern const int g;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

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

	/*Õ¾Á¢×´Ì¬Í¼ÏñÇÐÆ¬*/
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	/*ÐÐ×ß×´Ì¬Í¼ÏñÇÐÆ¬*/
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;
};

class Player
{
public:
	//The dimensions of the Player
	static const int Player_WIDTH = 170;
	static const int Player_HEIGHT = 152;

	//The position of the Player
	int posX;
	int posY;

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

	/*Õ¾Á¢×´Ì¬Í¼ÏñÇÐÆ¬*/
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	/*ÐÐ×ß×´Ì¬Í¼ÏñÇÐÆ¬*/
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;
};
