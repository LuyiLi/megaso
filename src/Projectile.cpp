#include "pch.h"
#include "Projectile.h"
#include "global.h"
#include "Map.h"
#include "pocket.h"

extern Map mainMap;

Projectile::Projectile()
{
	//Initialize the offsets

	//Set collision box dimension
	mCollider.w = Projectile_WIDTH;
	mCollider.h = Projectile_HEIGHT;

	for (int i = 0; i < 16; i++)
	{
		rectArray[i].x = 0;
		rectArray[i].y = 0;
		rectArray[i].w = 33;
		rectArray[i].h = 33;
	}
}


Projectile::~Projectile()
{
}

void Projectile::create(int x, int y, int mousePosX, int mousePosY, int accessories)
{
	if (!isExitsting)
	{
		damage = 5 + 4 * accessories;
		color = accessories;
		mCollider.x = x + 20;
		mCollider.y = y + 10;
		posX = mCollider.x;
		posY = mCollider.y;
		mVelX = (mousePosX - 450) / 8;
		mVelY = (mousePosY - 300) / 8;
		isExitsting = true;
		existTime = 0;
	}
}

void Projectile::move()
{
	if (isExitsting)
	{
		existTime++;
		if (existTime > 15)
			isExitsting = false;
		mCollider.x += mVelX;
		posX = mCollider.x - 10;

		if (checkCollision())
		{
			isExitsting = false;
		}

		mCollider.y += mVelY;
		posY = mCollider.y;

		//If the Projectile collided
		if (checkCollision())
		{
			isExitsting = false;
		}

		if (blockPosY != mCollider.y / (33) || blockPosX != mCollider.x / (33))
		{
			blockPosX = mCollider.x / (33);
			blockPosY = mCollider.y / (33);
			updateCollisionBox();

		}
	}
}

bool Projectile::checkCollision()
{
	for (int i = 0; i < 16; i++)
	{
		if (rectArray[i].x == 0 && rectArray[i].y == 0)
			continue;
		if (intersect(mCollider, rectArray[i]))
			return true;
	}
	if (mCollider.x <= 0 || mCollider.x >= 165000)
	{
		return true;
	}
	return false;
}
//update the Projectile's surrounding collision box
void Projectile::updateCollisionBox()
{
	int startBlockX, startBlockY;
	startBlockX = blockPosY - 1;
	startBlockY = blockPosX - 1;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (mainMap.mapData[startBlockX + i][startBlockY + j])
			{
				rectArray[i + 4 * j].x = (33) * (startBlockY + j);
				rectArray[i + 4 * j].y = (33) * (startBlockX + i);
			}
			else
			{
				rectArray[i + 4 * j].x = 0;
				rectArray[i + 4 * j].y = 0;
			}
		}
}

void Projectile::moveAction(int deltaX, int deltaY)
{
	if (isExitsting)
	{
		SDL_Rect temp = mCollider;
		temp.x += deltaX;
		temp.y += deltaY;
		SDL_RenderDrawRect(gRenderer, &temp);
		switch (color)
		{
		case 0:
			projectile_texture.render((posX + deltaX), (posY + deltaY), &projectile_clips[0], 0, NULL, SDL_FLIP_NONE, 2);
			break;
		case 1:
			projectile_texture_blue.render((posX + deltaX), (posY + deltaY), &projectile_clips[0], 0, NULL, SDL_FLIP_NONE, 2);
			break;
		case 2:
			projectile_texture_green.render((posX + deltaX), (posY + deltaY), &projectile_clips[0], 0, NULL, SDL_FLIP_NONE, 2);
			break;
		case 3:
			projectile_texture_red.render((posX + deltaX), (posY + deltaY), &projectile_clips[0], 0, NULL, SDL_FLIP_NONE, 2);
			break;
		default:
			break;
		}
	}
}

bool Projectile::loadTexture()
{
	if (!projectile_texture.loadFromFile("images/projectile.png")
		|| !projectile_texture_blue.loadFromFile("images/projectile1.png")
		|| !projectile_texture_green.loadFromFile("images/projectile2.png")
		|| !projectile_texture_red.loadFromFile("images/projectile3.png"))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	else
	{
		//Set sprite clips
		projectile_clips[0].x = 0;
		projectile_clips[0].y = 0;
		projectile_clips[0].w = 50;
		projectile_clips[0].h = 50;
	}
	
}
