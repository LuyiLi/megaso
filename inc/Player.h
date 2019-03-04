#pragma once
#include "LTexture.h"
//Åö×²µãÀà
extern int pos_x;
extern int pos_y;
extern const int g;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Player
{
public:
	//The dimensions of the Player
	static const int Player_WIDTH = 170;
	static const int Player_HEIGHT = 152;

	//Maximum axis velocity of the Player
	static const int Player_VEL = 5;

	//Initializes the variables
	Player();

	//Takes key presses and adjusts the Player's velocity
	void handleEvent(SDL_Event& e);

	//Moves the Player and checks collision
	void move(SDL_Rect& wall);

	void moveAction();
	bool initPlayerTexture();

	//Shows the Player on the screen
	void render(int camX, int camY);

	int getPosX();
	int getPosY();

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
