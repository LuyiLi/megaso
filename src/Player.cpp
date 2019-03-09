#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Player.h"
#include "global.h"
#include "Camera.h"
#include "SavingControl.h"
#include "Map.h"
#include "pocket.h"

extern Map mainMap;
extern pocket mainPocket;

Player::Player()
{
	//Initialize the offsets

	//Set collision box dimension
	mCollider.w = Player_WIDTH/2;
	mCollider.h = Player_HEIGHT/2;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	canJump = true;

	for (int i = 0; i < 16; i++)
	{
		rectArray[i].x = 0;
		rectArray[i].y = 0;
		rectArray[i].w = 50;
		rectArray[i].h = 50;
	}
}

void Player::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//if "w" or "SPACE" is pressed
		if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_SPACE)
		{
			if (canJump)
			{
				mVelY = -15;
				//Prevent player from jumping twice
				canJump = false;
			}
		}
		else if (e.key.keysym.sym == SDLK_a)
		{
			acceleration--;
		}
		else if (e.key.keysym.sym == SDLK_d)
		{
			acceleration++;
		}

	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_a: mVelX = 0; acceleration++; break;
		case SDLK_d: mVelX = 0; acceleration--;  break;
		}
	}
}

void Player::move()
{
	if (!acceleration)
		mVelX = 0;

	mCollider.x += mVelX;
	posX = mCollider.x - 10;

	if (checkCollision())
	{
		//Move back
		mCollider.x -= mVelX;
		mVelX = 0;
		posX = mCollider.x - 10;
	}
	else if(abs(mVelX) <= Player_VEL)
		mVelX += acceleration;

	//Move the Player up or down
	mCollider.y += mVelY;
	posY = mCollider.y;

	//If the Player collided
	if (checkCollision())
	{
		//Move back
		mCollider.y -= mVelY;
		mVelY = 0;
		posY = mCollider.y;
		canJump = true;
	}
	else
	{
		if (abs(mVelY) < 25)
			mVelY += g;
		if (mVelY > 5)
			canJump = false;
	}
	//Update collision box if moved out of current box
	if (blockPosY != mCollider.y / 50 || blockPosX != mCollider.x / 50)
	{
		blockPosX = mCollider.x / 50;
		blockPosY = mCollider.y / 50;
		updateCollisionBox();
	}
}

bool Player::checkCollision()
{
	for (int i = 0; i < 16; i++)
	{
		if (rectArray[i].x == 0 && rectArray[i].y == 0)
			continue;
		if (intersect(mCollider, rectArray[i]))
			return true;
	}

	return false;
}

void Player::pickUpItem(droppedItem *droppeditem)
{
	if (droppeditem->item.itemType != ITEM_NULL)
		if (intersect(mCollider, droppeditem->mCollider))
		{
			mainPocket.pocketUpdate();
			//todo :put it inside the backpack
			int existFlag = 0;
			for (int i = 0; i < 10; i++)
			{
				if (mainPocket.pocketData[0][i] == droppeditem->item.ID)
				{
					mainPocket.pocketData[1][i]++;
					existFlag = 1;
					droppeditem->deleteItem();
					break;
				}
			}
			if (!existFlag)
			{
				for (int i = 0; i < 10; i++)
				{
					if (mainPocket.pocketData[0][i] == 0)
					{
						mainPocket.pocketData[0][i] = droppeditem->item.ID;
						mainPocket.pocketData[1][i]++;
						droppeditem->deleteItem();
						break;
					}
				}
			}
		}

}

void Player::updateCollisionBox()
{
	int startBlockX, startBlockY;
	startBlockX = blockPosY - 1;
	startBlockY = blockPosX - 1;

	for (int i = 0; i<4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (mainMap.mapData[startBlockX + i][startBlockY + j])
			{
				rectArray[i + 4 * j].x = 50 * (startBlockY + j);
				rectArray[i + 4 * j].y = 50 * (startBlockX + i);
			}
			else
			{
				rectArray[i + 4 * j].x = 0;
				rectArray[i + 4 * j].y = 0;
			}
		}
}

/*
void Player::render(int camX, int camY)
{
	//Show the dot relative to the camera
	slime_standing_texture.render(posX - camX, posY - camY);
	slime_walking_texture.render(posX - camX, posY - camY);
}
*/

void Player::moveAction(int deltaX, int deltaY)
{

	static int frame_walk = 0;
	static int frame_stand = 0;
	if (acceleration > 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE,4);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}

	}
	else if (acceleration < 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL,4);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}
	}
	else
	{
		SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 6];
		slime_standing_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE,4);
		++frame_stand;
		if (frame_stand / 6 >= 6)
		{
			frame_stand = 0;
		}
	}
}

bool Player::loadTexture()
{
	if (!slime_walking_texture.loadFromFile("images/slime_walk.png"))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	else
	{
		//Set sprite clips
		slime_walk_clips[0].x = 0;
		slime_walk_clips[0].y = 0;
		slime_walk_clips[0].w = 416;
		slime_walk_clips[0].h = 304;

		slime_walk_clips[1].x = 416;
		slime_walk_clips[1].y = 0;
		slime_walk_clips[1].w = 416;
		slime_walk_clips[1].h = 304;

		slime_walk_clips[2].x = 832;
		slime_walk_clips[2].y = 0;
		slime_walk_clips[2].w = 416;
		slime_walk_clips[2].h = 304;

		slime_walk_clips[3].x = 1248;
		slime_walk_clips[3].y = 0;
		slime_walk_clips[3].w = 416;
		slime_walk_clips[3].h = 304;
	}
	//Load sprite sheet texture
	if (!slime_standing_texture.loadFromFile("images/slime_stand.png"))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	else
	{
		//Set sprite clips
		slime_stand_clips[0].x = 0;
		slime_stand_clips[0].y = 0;
		slime_stand_clips[0].w = 416;
		slime_stand_clips[0].h = 304;

		slime_stand_clips[1].x = 416;
		slime_stand_clips[1].y = 0;
		slime_stand_clips[1].w = 416;
		slime_stand_clips[1].h = 304;

		slime_stand_clips[2].x = 832;
		slime_stand_clips[2].y = 0;
		slime_stand_clips[2].w = 416;
		slime_stand_clips[2].h = 304;

		slime_stand_clips[3].x = 1248;
		slime_stand_clips[3].y = 0;
		slime_stand_clips[3].w = 416;
		slime_stand_clips[3].h = 304;

		slime_stand_clips[4].x = 1664;
		slime_stand_clips[4].y = 0;
		slime_stand_clips[4].w = 416;
		slime_stand_clips[4].h = 304;

		slime_stand_clips[5].x = 2080;
		slime_stand_clips[5].y = 0;
		slime_stand_clips[5].w = 416;
		slime_stand_clips[5].h = 304;
	}
}
