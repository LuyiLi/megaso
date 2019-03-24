#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Player.h"
#include "Camera.h"
#include "SavingControl.h"
#include "Map.h"
#include "pocket.h"
#include "global.h"
#include "Entity.h"
#include <math.h>

extern Map mainMap;
extern pocket mainPocket;
extern SDL_Point centralPoint[4];
extern int direction;
//extern Uint32 renderBgChangeCallback(Uint32 interval, void* param);
extern int mouseState;

Player::Player()
{
	//Initialize the offsets

	//Set collision box dimension
	mCollider.w = Player_WIDTH/3;
	mCollider.h = Player_HEIGHT/3;
	healthPoint = 100;
	healthLimit = 100;
	magicPoint = 100;
	magicLimit = 100;
	isDead = 0;
	haveLight;
	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	canJump = true;
	hitFlag = 0;
	weaponState = 0;
	for (int i = 0; i < 16; i++)
	{
		rectArray[i].x = 0;
		rectArray[i].y = 0;
		rectArray[i].w = 33;
		rectArray[i].h = 33;
	}
}
//control the movement of reimu rubo through the keyboard
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
		case SDLK_a: 
				mVelX = 0; 
				acceleration++; 
				break;
			
		case SDLK_d: 
				mVelX = 0; 
				acceleration--;  
				break;
			
		}
	}
}

void Player::move()
{
	static int healthFlag = 0;
	static int magicFlag = 0;
	if (healthPoint < 100)
	{
		healthFlag++;
		if (healthFlag >= 30)
		{
			healthFlag = 0;
			healthPoint++;
		}
	}
	if (magicPoint < 100)
	{
		magicFlag++;
		if (magicFlag >= 10)
		{
			magicFlag = 0;
			magicPoint++;
		}
	}

	if (!acceleration && mVelX != 0)
		mVelX = mVelX > 0 ? mVelX - 1 : mVelX + 1;

	mCollider.x += mVelX;
	posX = mCollider.x - 10;

	if (checkCollision())
	{
		//Move back
		mCollider.x -= mVelX;
		mVelX = 0;
		posX = mCollider.x - 10;
	}
	else if (((mVelX <= Player_VEL || acceleration < 0) && mVelX >= 0) || ((mVelX >= -Player_VEL || acceleration > 0) && mVelX <= 0))
	{
		mVelX += acceleration;
	}
	else
	{
		mVelX -= acceleration;
	}
	
	
	//Move the Player up or down
	mCollider.y += mVelY >= 0 && mVelY < 1 ? 1 : mVelY;
	posY = mCollider.y;

	//If the Player collided
	if (checkCollision())
	{
		//Move back
		mCollider.y -= mVelY >= 0 && mVelY < 1 ? 1 : mVelY;
		mVelY = 0;
		posY = mCollider.y;
		canJump = true;
	}
	else
	{ 
		//block drops
		if (abs(mVelY) < 25)
			mVelY += g;
		if (mVelY > 5)
			canJump = false;
	}
	if (blockPosY != mCollider.y / (33) || blockPosX != mCollider.x / (33))
	{
		blockPosX = mCollider.x / (33);
		blockPosY = mCollider.y / (33);
		updateCollisionBox();
		
	}
	if (!canBeHit)
	{
		hitFlag++;
		if (hitFlag > 10)
			canBeHit = true;
	}

	//move the weapon in hand
	if (isUsing && currentItem.itemType == ITEM_WEAPON)
	{
		double angleY = sin(0.017453 * (currentAngle - 45));
		double angleX = direction ? cos(0.017453 * (currentAngle - 45)) : -cos(0.017453 * (currentAngle - 45));
		for (int i = 0; i < 5; i++)
		{
			weaponCollisionPoints[i].x = posX + centralPoint[direction].x + 18*i*angleX;
			weaponCollisionPoints[i].y = posY - 20 + centralPoint[direction].y + 18*i*angleY;
		}
	}
	else
	{
		if(weaponCollisionPoints[0].y != 0)
			for (int i = 0; i < 5; i++)
			{
				weaponCollisionPoints[i].y = 0;
			}
	}
}

void Player::getHit(Enemy *enemy)
{
	if (canBeHit)
	{
		if (intersect(enemy->mCollider, mCollider))
		{
			mVelX = enemy->mCollider.x < mCollider.x ? 14 : -14;
			if (mVelY > -2)
				mVelY -= 6;
			canBeHit = false;
			hitFlag = 0;
			if (healthPoint - enemy->enemyData->damage >= 0)
			{
				healthPoint -= enemy->enemyData->damage;
			}
			else
			{
				getKilled();
			}
		}
	}
}

bool Player::checkCollision()
{
	for (int i = 0; i < 20; i++)
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

void Player::getKilled()
{
	isDead = 1;
	
	mCollider.x = 82500;
	mCollider.y = 4000;
	mVelX = 0;
	mVelY = 0;
	
	healthPoint = healthLimit;
}

void Player::pickUpItem(droppedItem *droppeditem)
{
	if (droppeditem->item.itemType != ITEM_NULL)
		//player touches the droppeditem
		if (intersect(mCollider, droppeditem->mCollider))
		{
			mainPocket.pocketUpdate();
			//todo :put it inside the backpack
			int existFlag = 0;
			for (int i = 0; i < 40; i++)
			{
				if (mainPocket.pocketData[0][i] == droppeditem->item.ID&&mainPocket.pocketData[1][i]<=98)
				{
					mainPocket.pocketData[1][i]++;
					existFlag = 1;
					droppeditem->deleteItem();
					break;
				}
			}
			//if the droppeditem is not exist , create a type
			if (!existFlag)
			{
				for (int i = 0; i < 40; i++)
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
//update the player's surrounding collision box
void Player::updateCollisionBox()
{
	int startBlockX, startBlockY;
	startBlockX = blockPosY - 1;
	startBlockY = blockPosX - 1;

	for (int i = 0; i<4; i++)
		for (int j = 0; j < 5; j++)
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

void Player::moveAction(int deltaX, int deltaY, int posX, int posY)
{

	static int frame_walk = 0;
	static int frame_stand = 0;
	int R = mainMap.lightBlock[21][21], G = mainMap.lightBlock[21][21], B = mainMap.lightBlock[21][21];

	slime_walking_texture.setColor(R, G, B);
	slime_walking_texture_blue.setColor(R, G, B);
	slime_walking_texture_green.setColor(R, G, B);
	slime_walking_texture_red.setColor(R, G, B);

	slime_standing_texture.setColor(R, G, B);
	slime_standing_texture_blue.setColor(R, G, B);
	slime_standing_texture_green.setColor(R, G, B);
	slime_standing_texture_red.setColor(R, G, B);
	
	slime_standing_side_texture.setColor(R, G, B);
	slime_standing_side_texture_blue.setColor(R, G, B);
	slime_standing_side_texture_red.setColor(R, G, B);
	slime_standing_side_texture_green.setColor(R, G, B);

	if (acceleration > 0)
	{
		
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 8];
		
		switch (mainPocket.accessories)
		{
		case 0:
			slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
			break;
		case 1:
			slime_walking_texture_blue.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
			break;
		case 2:
			slime_walking_texture_green.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
			break;
		case 3:
			slime_walking_texture_red.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
			break;
		default:
			break;
		}

		
		++frame_walk;
		if (frame_walk / 8 >= 4)
		{
			frame_walk = 0;
		}
	}
	else if (acceleration < 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 8];
		switch (mainPocket.accessories)
		{
		case 0:
			slime_walking_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
			break;
		case 1:
			slime_walking_texture_blue.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
			break;
		case 2:
			slime_walking_texture_green.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
			break;
		case 3:
			slime_walking_texture_red.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
			break;
		default:
			break;
		}
		++frame_walk;
		if (frame_walk / 8 >= 4)
		{
			frame_walk = 0;
		}
	}
	else
	{
		if (weaponState == 0)
		{
			SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 12];
			switch (mainPocket.accessories)
			{
			case 0:
				slime_standing_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 12);
				break;
			case 1:
				slime_standing_texture_blue.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 12);
				break;
			case 2:
				slime_standing_texture_green.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 12);
				break;
			case 3:
				slime_standing_texture_red.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 12);
				break;
			default:
				break;
			}
			++frame_stand;
			if (frame_stand / 12 >= 6)
			{
				frame_stand = 0;
			}
		}
		else if (weaponState == 1)
		{
			SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 12];
			switch (mainPocket.accessories)
			{
			case 0:
				slime_standing_side_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
				break;
			case 1:
				slime_standing_side_texture_blue.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
				break;
			case 2:
				slime_standing_side_texture_green.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
				break;
			case 3:
				slime_standing_side_texture_red.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_NONE, 6);
				break;
			default:
				break;
			}
			++frame_stand;
			if (frame_stand / 12 >= 6)
			{
				frame_stand = 0;
			}
		}
		else if (weaponState == 2)
		{
			SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 12];
			switch (mainPocket.accessories)
			{
			case 0:
				slime_standing_side_texture.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
				break;
			case 1:
				slime_standing_side_texture_blue.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
				break;
			case 2:
				slime_standing_side_texture_green.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
				break;
			case 3:
				slime_standing_side_texture_red.render((posX + deltaX), (posY + deltaY), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL, 6);
				break;
			default:
				break;
			}
			++frame_stand;
			if (frame_stand / 12 >= 6)
			{
				frame_stand = 0;
			}
		}
		
	}
	
}

bool Player::loadTexture()
{
	if (!slime_walking_texture.loadFromFile("images/slime_walk.png")
		|| !slime_walking_texture_blue.loadFromFile("images/blue_2.png")
		|| !slime_walking_texture_green.loadFromFile("images/green_2.png")
		|| !slime_walking_texture_red.loadFromFile("images/red_2.png"))
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
	if (!slime_standing_texture.loadFromFile("images/slime_stand.png")
		|| !slime_standing_texture_blue.loadFromFile("images/blue_3.png")
		|| !slime_standing_texture_green.loadFromFile("images/green_3.png")
		|| !slime_standing_texture_red.loadFromFile("images/red_3.png"))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	if (!slime_standing_side_texture.loadFromFile("images/slime_stand_side.png")
		|| !slime_standing_side_texture_blue.loadFromFile("images/blue_1.png")
		|| !slime_standing_side_texture_green.loadFromFile("images/green_1.png")
		|| !slime_standing_side_texture_red.loadFromFile("images/red_1.png"))
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
