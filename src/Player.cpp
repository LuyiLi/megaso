#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Player.h"
#include "global.h"


Player::Player()
{
	//Initialize the offsets
	//pos_x = 0;
	//pos_y = 0;

	//Set collision box dimension
	mCollider.w = Player_WIDTH;
	mCollider.h = Player_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void Player::handleEvent(SDL_Event& e)
{
	SDL_Rect wall;
	wall.x = 0;
	wall.y = 260;
	wall.w = 1000;
	wall.h = 400;
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			if (checkCollision(mCollider, wall))
			{
				mVelY = -20;
			}

			break;
		case SDLK_LEFT: mVelX -= Player_VEL; break;
		case SDLK_RIGHT: mVelX += Player_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: mVelX += Player_VEL; break;
		case SDLK_RIGHT: mVelX -= Player_VEL; break;
		}
	}
}

void Player::move(SDL_Rect& wall)
{
	//Move the Player left or right
	pos_x += mVelX;
	mCollider.x = pos_x;
	if (!checkCollision(mCollider, wall))
	{
		mVelY += g;
	}

	if ((pos_x < 0) || (pos_x + Player_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, wall))
	{
		//Move back
		pos_x -= mVelX;
		mCollider.x = pos_x;
	}

	//Move the Player up or down
	pos_y += mVelY;
	mCollider.y = pos_y;

	//If the Player collided or went too far up or down
	if ((pos_y < 0) || (pos_y + Player_HEIGHT > SCREEN_HEIGHT) || checkCollision(mCollider, wall))
	{
		//Move back
		pos_y -= mVelY;
		mVelY = 0;
		mCollider.y = pos_y;
	}

}