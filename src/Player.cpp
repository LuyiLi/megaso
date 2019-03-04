#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Player.h"
#include "global.h"

extern int pos_x;
extern int pos_y;

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
		case SDLK_LEFT: mVelX = 0; break;
		case SDLK_RIGHT: mVelX = 0; break;
		}
	}
}

void Player::move(SDL_Rect& wall)
{
	//Move the Player left or right
	pos_x += mVelX;
	mCollider.x = pos_x+19;
	if (!checkCollision(mCollider, wall))
	{
		mVelY += g;
	}

	if (checkCollision(mCollider, wall))
	{
		//Move back
		pos_x -= mVelX;
		mVelX = 0;
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

/*
void Player::render(int camX, int camY)
{
	//Show the dot relative to the camera
	slime_standing_texture.render(pos_x - camX, pos_y - camY);
	slime_walking_texture.render(pos_x - camX, pos_y - camY);
}
*/

void Player::moveAction()
{
	
	static int frame_walk = 0;
	static int frame_stand = 0;
	if (mVelX > 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}

	}
	else if (mVelX < 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}
	}
	else
	{
		SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 6];
		slime_standing_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
		++frame_stand;
		if (frame_stand / 6 >= 6)
		{
			frame_stand = 0;
		}
	}
}

bool Player::initPlayerTexture()
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
	return pos_x;
}

int Player::getPosY()
{
	return pos_y;
}