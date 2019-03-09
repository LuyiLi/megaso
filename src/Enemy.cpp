#include "pch.h"
#include "Enemy.h"
#include "global.h"
#include "Map.h"

extern Map mainMap;

Enemy::Enemy()
{
	//Initialize the offsets

	//Set collision box dimension
	mCollider.w = Enemy_WIDTH / 2;
	mCollider.h = Enemy_HEIGHT / 2;

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

void Enemy::move()
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
	else if (abs(mVelX) <= Enemy_VEL)
		mVelX += acceleration;


	//Move the Enemy up or down
	mCollider.y += mVelY;
	posY = mCollider.y;

	//If the Enemy collided
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
	if (blockPosY != mCollider.y / 50 || blockPosX != mCollider.x / 50)
	{
		blockPosX = mCollider.x / 50;
		blockPosY = mCollider.y / 50;
		updateCollisionBox();
	}
}

void Enemy::dealDamage(Player *player)
{

}

bool Enemy::checkCollision()
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

void Enemy::updateCollisionBox()
{
	int startBlockX, startBlockY;
	startBlockX = blockPosY - 1;
	startBlockY = blockPosX - 1;

	for (int i = 0; i < 4; i++)
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
void Enemy::render(int camX, int camY)
{
	//Show the dot relative to the camera
	enemy_standing_texture.render(posX - camX, posY - camY);
	enemy_walking_texture.render(posX - camX, posY - camY);
}
*/

void Enemy::moveAction(int deltaX, int deltaY)
{

	static int frame_walk = 0;
	static int frame_stand = 0;
	if (acceleration > 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 4);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}

	}
	else if (acceleration < 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 4);
		++frame_walk;
		if (frame_walk / 4 >= 4)
		{
			frame_walk = 0;
		}
	}
	else
	{
		SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 6];
		slime_standing_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 4);
		++frame_stand;
		if (frame_stand / 6 >= 6)
		{
			frame_stand = 0;
		}
	}
}

bool Enemy::loadTexture()
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

int Enemy::getPosX()
{
	return posX;
}

int Enemy::getPosY()
{
	return posY;
}

int Enemy::getVelX()
{
	return mVelX;
}

int Enemy::getVelY()
{
	return mVelY;
}