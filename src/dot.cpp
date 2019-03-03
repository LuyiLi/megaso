#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "dot.h"
#include "global.h"


Dot::Dot()
{
	//Initialize the offsets
	//pos_x = 0;
	//pos_y = 0;

	//Set collision box dimension
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void Dot::handleEvent(SDL_Event& e)
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
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; break;
		case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void Dot::move(SDL_Rect& wall)
{
	//Move the dot left or right
	pos_x += mVelX;
	mCollider.x = pos_x;
	if (!checkCollision(mCollider, wall))
	{
		mVelY += g;
	}

	if ((pos_x < 0) || (pos_x + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, wall))
	{
		//Move back
		pos_x -= mVelX;
		mCollider.x = pos_x;
	}

	//Move the dot up or down
	pos_y += mVelY;
	mCollider.y = pos_y;

	//If the dot collided or went too far up or down
	if ((pos_y < 0) || (pos_y + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(mCollider, wall))
	{
		//Move back
		pos_y -= mVelY;
		mCollider.y = pos_y;
	}

}