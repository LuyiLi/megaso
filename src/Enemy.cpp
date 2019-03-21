#include "pch.h"
#include "Entity.h"
#include "global.h"
#include "Map.h"
#include "droppedItem.h"

extern Map mainMap;
extern Player player;
extern droppedItem droppedItemList[200];
extern Item itemList[500];

Enemy::Enemy()
{
	//Initialize the offsets
	//Set collision box dimension
	Enemy_VEL = 3;
	mCollider.x = 0;
	mCollider.y = 0;
	canBeHit = true;
	canBeKnockedBack = true;
	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	canJump = true;
	attackMode = ATTACKMODE_NONE;
	isAlive = false;

	for (int i = 0; i < 36; i++)
	{
		rectArray[i].x = 0;
		rectArray[i].y = 0;
		rectArray[i].w = 33;
		rectArray[i].h = 33;
	}
}

void Enemy::create(int x, int y, EnemyData *data)
{
	enemyData = data;
	Enemy_VEL = enemyData->Enemy_VEL;
	mCollider.w = enemyData->Enemy_WIDTH;
	mCollider.h = enemyData->Enemy_HEIGHT / 2;
	mCollider.x = x;
	mCollider.y = y;
	canBeHit = true;
	canBeKnockedBack = true;
	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	canJump = true;
	attackMode = ATTACKMODE_NONE;
	isAlive = true;
	healthPoint = enemyData->healthLimit;
	for (int i = 0; i < 36; i++)
	{
		rectArray[i].x = 0;
		rectArray[i].y = 0;
		rectArray[i].w = 33;
		rectArray[i].h = 33;
	}
}

void Enemy::move()
{
	if (!isAlive)
		return;
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


	//If the Player collided
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
	if (blockPosY != mCollider.y / 33 || blockPosX != mCollider.x / 33)
	{
		blockPosX = mCollider.x / 33;
		blockPosY = mCollider.y / 33;
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
	if (!isAlive)
		return;
	if (canBeHit)
	{
		for (int i = 0; i < 5; i++)
			if (inRect(player->weaponCollisionPoints[i], mCollider))
			{
				healthPoint -= player->currentItem.weaponDamage;
				if (canBeKnockedBack)
				{
					mVelX = player->mCollider.x < mCollider.x ? 10 : -10;
					if (mVelY > -2)
						mVelY -= 9;
					canBeHit = false;
					hitFlag = 0;
				}
				if (healthPoint < 0)
					getKilled();
				return;
			}
	}
}

void Enemy::getHitProjectile(Projectile *projectile)
{
	if (isAlive && projectile->isExitsting)
		if (intersect(mCollider, projectile->mCollider))
		{
			projectile->isExitsting = false;
			healthPoint -= 5;
			if (canBeKnockedBack)
			{
				mVelX = projectile->mVelX > 0 ? 10 : -10;
				if (mVelY > -2)
					mVelY -= 9;
				canBeHit = false;
				hitFlag = 0;
			}
			if (healthPoint < 0)
				getKilled();
		}
}

void Enemy::getKilled()
{
	for (int i = 0; i < 200; i++)
		if (droppedItemList[i].item.itemType == ITEM_NULL)
		{
			droppedItemList[i].create(mCollider.x + 15, mCollider.y, itemList[enemyData->dropID]);
			break;
		}
	isAlive = false;
	mCollider.x = 0;
	mCollider.y = 0;
}

bool Enemy::checkCollision()
{
	
	for (int i = 0; i < 36; i++)
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

	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
		{
			if (mainMap.mapData[startBlockX + i][startBlockY + j])
			{
				rectArray[i + 6 * j].x = 33 * (startBlockY + j);
				rectArray[i + 6 * j].y = 33 * (startBlockX + i);
			}
			else
			{
				rectArray[i + 6 * j].x = 0;
				rectArray[i + 6 * j].y = 0;
			}
		}
}

void Enemy::moveAction(int deltaX, int deltaY)
{
	if (!isAlive)
		return;

	SDL_Point enemyCenter;
	int lightX = posX / 33 - player.blockPosX + 20;
	int lightY = posY / 33 - player.blockPosY + 20;
	int light = 1.3*mainMap.lightBlock[lightX][lightY] < 255 ? 1.3*mainMap.lightBlock[lightX][lightY] : 255;
	enemyData->enemy_walking_texture[0].setColor(light, light, light);
	enemyData->enemy_walking_texture[1].setColor(light, light, light);

	if (acceleration > 0 && attackMode == ATTACKMODE_PREPARE)
	{
		frameFlag++;
		if (frameFlag == 2)
		{
			frame++;
			frameFlag = 0;

		}
		if (frame == 5)
		{
			frame = 0;
			attackMode = ATTACKMODE_ATTACKING;
		}
		else
		{
			SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][frame];
			enemyData->enemy_walking_texture[0].render((posX + deltaX), (-33 + posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 0.55);
			
		}
	}

	if (acceleration < 0 && attackMode == ATTACKMODE_PREPARE)
	{
		frameFlag++;
		if (frameFlag == 2)
		{
			frame++;
			frameFlag = 0;

		}
		if (frame == 5)
		{
			frame = 0;
			attackMode = ATTACKMODE_ATTACKING;
		}
		else
		{
			SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][frame];
			enemyData->enemy_walking_texture[0].render((posX + deltaX), (-33 + posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 0.55);

		}
	}

	if (acceleration < 0 && attackMode == ATTACKMODE_FINISH)
	{
		frameFlag++;
		if (frameFlag == 2)
		{
			frame++;
			frameFlag = 0;

		}
		if (frame == 5)
		{
			frame = 0;
			attackMode = ATTACKMODE_NONE;
		}
		else
		{
			SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][abs(frame-4)];
			enemyData->enemy_walking_texture[0].render((posX + deltaX), (-33 + posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 0.55);
			
		}
	}

	if (acceleration > 0 && attackMode == ATTACKMODE_FINISH)
	{
		frameFlag++;
		if (frameFlag == 2)
		{
			frame++;
			frameFlag = 0;

		}
		if (frame == 5)
		{
			frame = 0;
			attackMode = ATTACKMODE_NONE;
		}
		else
		{
			SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][abs(frame-4)];
			enemyData->enemy_walking_texture[0].render((posX + deltaX), (-33 + posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 0.55);

		}
	}

	if (acceleration > 0 && attackMode==ATTACKMODE_NONE)
	{
		mCollider.h = enemyData->Enemy_HEIGHT / 2;
		mCollider.w = enemyData->Enemy_WIDTH ;
		SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][frame_walk/10+5];
		enemyData->enemy_walking_texture[0].render((posX + deltaX), (-33+posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 0.55);
		if (frame_walk / 10 >= 3)
		{
			frame_walk = 0;
		}
		else
		{
			frame_walk++;
		}
	}
	if (acceleration < 0 && attackMode == ATTACKMODE_NONE)
	{
		mCollider.h = enemyData->Enemy_HEIGHT / 2;
		mCollider.w = enemyData->Enemy_WIDTH;
		SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][frame_walk / 10 + 5];
		enemyData->enemy_walking_texture[0].render((posX + deltaX), (-33+posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 0.55);
		if (frame_walk / 10 >= 3)
		{
			frame_walk = 0;
		}
		else
		{
			frame_walk++;
		}
	}

	if (acceleration > 0&&attackMode==ATTACKMODE_ATTACKING)
	{
		mCollider.h = enemyData->Enemy_HEIGHT / 2;
		mCollider.w = enemyData->Enemy_WIDTH / 2;
		enemyCenter.x = 42;
		enemyCenter.y = 42;
		SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][0];
		enemyData->enemy_walking_texture[1].render((posX + deltaX), (posY + deltaY), currentClip, angle, &enemyCenter, SDL_FLIP_NONE, 1.2);
		angle += 30;

	}
	else if(acceleration < 0 && attackMode == ATTACKMODE_ATTACKING)
	{
		mCollider.h = enemyData->Enemy_HEIGHT / 2;
		mCollider.w = enemyData->Enemy_WIDTH / 2;
		enemyCenter.x = 42;
		enemyCenter.y = 42;
		SDL_Rect* currentClip = &enemyData->enemy_walk_clips[0][0];
		enemyData->enemy_walking_texture[1].render((posX + deltaX), (posY + deltaY), currentClip, angle, &enemyCenter, SDL_FLIP_HORIZONTAL, 1.2);
		angle -= 30;
	}
	if (modeFlag < 200)
	{
		modeFlag++;
	}
	else 
	{
		if (attackMode == ATTACKMODE_NONE)
		{
			attackMode = ATTACKMODE_PREPARE;
			Enemy_VEL = 8;
			modeFlag = 0;
		}
		else if (attackMode == ATTACKMODE_ATTACKING)
		{
			attackMode = ATTACKMODE_FINISH;
			Enemy_VEL = 1;
			modeFlag = 0;
		}
		
	}
	if (healthPoint < enemyData->healthLimit)
	{
		double percentage = (double)healthPoint / (double)enemyData->healthLimit;
		enemyData->enemyHp_texture.render(0 + posX + deltaX + 40, posY + deltaY - 40, &enemyData->enemyHp_clips[2], 0, NULL, SDL_FLIP_NONE, 2);
		enemyData->enemyHp_texture.renderWithScale(0 + posX + deltaX + 40, posY + deltaY - 40, &enemyData->enemyHp_clips[0], 0, NULL, SDL_FLIP_NONE, 2, 2, percentage);
		enemyData->enemyHp_texture.render((50 * percentage) - 7 + posX + deltaX + 40, posY + deltaY - 40, &enemyData->enemyHp_clips[1], 0, NULL, SDL_FLIP_NONE, 2);

	}
}

void Enemy::changeEnemyBehavior()
{
	switch (enemyData->AI)
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
