#include "pch.h"
#include "Entity.h"
#include "global.h"
#include "Map.h"

extern Map mainMap;
extern Player player;
SDL_Rect enemy_walk_clips[4];
LTexture enemy_walking_texture;

Enemy::Enemy()
{
	//Initialize the offsets

	//Set collision box dimension
	Enemy_VEL = 3;
	mCollider.w = Enemy_WIDTH / 2;
	mCollider.h = Enemy_HEIGHT / 2;
	mCollider.x = 2000;
	mCollider.y = 2000;
	canBeHit = true;
	canBeKnockedBack = true;
	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	canJump = true;
	damage = 5;
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
	changeEnemyBehavior();
	mCollider.x += mVelX;
	posX = mCollider.x - 10;

	if (checkCollision())
	{
		//Move back
		mCollider.x -= mVelX;
		mVelX = 0;
		posX = mCollider.x - 10;
	}
	else if (((mVelX <= Enemy_VEL || acceleration < 0) && mVelX >= 0) || ((mVelX >= -Enemy_VEL || acceleration > 0) && mVelX <= 0))
	{
		mVelX += acceleration;
	}
	else
	{
		mVelX -= acceleration;
	}

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
	if (blockPosY != mCollider.y / 50 || blockPosX != mCollider.x / 50)
	{
		blockPosX = mCollider.x / 50;
		blockPosY = mCollider.y / 50;
		updateCollisionBox();
	}
	if (!canBeHit)
	{
		hitFlag++;
		if (hitFlag > 7)
			canBeHit = true;
	}
}

void Enemy::getHit(Player *player)
{
	if (canBeHit)
	{
		for (int i = 0; i < 5; i++)
			if (inRect(player->weaponCollisionPoints[i], mCollider))
			{
				healthPoint -= 1;
				//todo: Change the damage
				if (canBeKnockedBack)
				{
					mVelX = player->mCollider.x < mCollider.x ? 10 : -10;
					if (mVelY > -2)
						mVelY -= 9;
					canBeHit = false;
					hitFlag = 0;
				}
				return;
			}
	}
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

void Enemy::moveAction(int deltaX, int deltaY)
{

	static int angle = 0;
	SDL_Point enemyCenter;
	enemyCenter.x = 41;
	enemyCenter.y = 41;
	if (acceleration > 0)
	{
		SDL_Rect* currentClip = &enemy_walk_clips[0];
		enemy_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, angle, &enemyCenter, SDL_FLIP_NONE, 1.2);
		angle += 15;

	}
	else if(acceleration < 0)
	{
		SDL_Rect* currentClip = &enemy_walk_clips[0];
		enemy_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, angle, &enemyCenter, SDL_FLIP_HORIZONTAL, 1.2);
		angle -= 15;
	}
}

bool Enemy::loadTexture()
{
	if (!enemy_walking_texture.loadFromFile("images/pangolin_1.png"))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	else
	{
		//Set sprite clips
		enemy_walk_clips[0].x = 0;
		enemy_walk_clips[0].y = 0;
		enemy_walk_clips[0].w = 100;
		enemy_walk_clips[0].h = 100;
		return true;
	}
	//Load sprite sheet texture
}

void Enemy::changeEnemyBehavior()
{
	switch (AI)
	{
	case AI_WARRIOR:
		acceleration = player.mCollider.x - mCollider.x < 0 ? -1 : 1;
		if (canJump == true && !mVelX)
		{
			canJump = false;
			mVelY = -15;
		}
		break;
	case AI_PANGOLIN:
		break;
	default:
		break;
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