#pragma once
#include "item.h"

class droppedItem
{
public:
	droppedItem();
	~droppedItem();

	Item item;

	const int droppedItem_HEIGHT = 20;
	const int droppedItem_WIDTH = 20;
	//The position of the droppedItem
	int posX;
	int posY;
	int blockPosX;
	int blockPosY;

	SDL_Rect collisionRect;

	//Takes key presses and adjusts the droppedItem's velocity

	//Moves the droppedItem and checks collision
	void move();
	void create(int posX, int posY, Item item);
	void deleteItem();
	void moveAction(int, int);
	bool loadTexture();
	bool checkCollision();

	void updateCollisionBox();

	//Shows the droppedItem on the screen
	void render(int camX, int camY);

	//The velocity of the droppedItem
	int mVelY;

	//droppedItem's collision box
	SDL_Rect mCollider;

		/*Õ¾Á¢×´Ì¬Í¼ÏñÇÐÆ¬
		//const int standing_frames = 6;
		SDL_Rect slime_stand_clips[6];
		LTexture slime_standing_texture;
		/*ÐÐ×ß×´Ì¬Í¼ÏñÇÐÆ¬
		//const int walking_frames = 4;
		SDL_Rect slime_walk_clips[4];
		LTexture slime_walking_texture;
	*/
};

