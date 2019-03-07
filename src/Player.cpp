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
extern Map mainMap;

Player::Player()
{
	//Initialize the offsets

	//Set collision box dimension
	mCollider.w = Player_WIDTH;
	mCollider.h = Player_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	canJump = true;

	for (int i = 0; i < 25; i++)
	{
		rectArray[i].x = 0;
		rectArray[i].y = 0;
		rectArray[i].w = 100;
		rectArray[i].h = 100;
	}
}

void Player::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			if (canJump)
			{
				mVelY = -21;
				canJump = false;
			}
			break;
		case SDLK_a: acceleration--; break;
		case SDLK_d: acceleration++; break;
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
	posX = mCollider.x - 19;

	if (checkCollision())
	{
		//Move back
		mCollider.x -= mVelX;
		mVelX = 0;
		posX = mCollider.x - 19;
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
	if (blockPosY != posY / 100 || blockPosX != posX / 100)
	{
		blockPosX = posX / 100;
		blockPosY = posY / 100;
		updateCollisionBox();
	}
}

bool Player::checkCollision()
{
	for (int i = 0; i < 25; i++)
	{
		if (rectArray[i].x == 0 && rectArray[i].y == 0)
			continue;
		if (intersect(mCollider, rectArray[i]))
			return true;
	}

	return false;
}

void Player::updateCollisionBox()
{
	int startBlockX, startBlockY;
	startBlockX = blockPosY - 2;
	startBlockY = blockPosX - 1;

	for (int i = 0; i<5; i++)
		for (int j = 0; j < 5; j++)
		{
			if (mainMap.mapData[startBlockX + i][startBlockY + j])
			{
				rectArray[i + 5 * j].x = 100 * (startBlockY + j);
				rectArray[i + 5 * j].y = 100 * (startBlockX + i);
			}
			else
			{
				rectArray[i + 5 * j].x = 0;
				rectArray[i + 5 * j].y = 0;
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
		slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}

	}
	else if (acceleration < 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}
	}
	else
	{
		SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 6];
		slime_standing_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE);
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

int Player::getPosX()
{
	return posX;
}

int Player::getPosY()
{
	return posY;
}

int Player::getVelX()
{
	return mVelX;
}

int Player :: getVelY()
{
	return mVelY;
}