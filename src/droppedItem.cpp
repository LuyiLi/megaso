#include "pch.h"
#include "droppedItem.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "global.h"
#include "Camera.h"
#include "Map.h"

extern Map mainMap;
extern const int g;
extern Item itemList[100];

droppedItem::droppedItem()
{
	//Initialize the offsets

	//Set collision box dimension
	mCollider.w = droppedItem_WIDTH;
	mCollider.h = droppedItem_HEIGHT;
	collisionRect.h = 50;
	collisionRect.w = 50;
	//Initialize the velocity
	mVelY = 0;

	//Set the default itemType to null
	item.itemType = ITEM_NULL;
}

droppedItem::~droppedItem()
{

}


void droppedItem::create(int posX, int posY, Item anItem)
{
	mCollider.x = posX;
	mCollider.y = posY;
	item = anItem;
	mVelY = 0;
}

void droppedItem::deleteItem()
{
	item = itemList[0];
}

void droppedItem::move()
{
	if (item.itemType != ITEM_NULL)
	{
		//Move the droppedItem up or down
		mCollider.y += mVelY;
		posY = mCollider.y;

		//If the droppedItem collided
		if (checkCollision())
		{
			//Move back
			mCollider.y -= mVelY;
			mVelY = 0;
			posY = mCollider.y;
		}
		else
		{
			if (abs(mVelY) < 25)
				mVelY += g;
		}
		if (blockPosY != mCollider.y / 50 || blockPosX != mCollider.x / 50)
		{
			blockPosX = mCollider.x / 50;
			blockPosY = mCollider.y / 50;
			updateCollisionBox();
		}
	}
}

bool droppedItem::checkCollision()
{
		if (collisionRect.x != 0 && collisionRect.y != 0)
			if (intersect(mCollider, collisionRect))
			return true;

	return false;
}

void droppedItem::updateCollisionBox()
{
	if (mainMap.mapData[blockPosY + 1][blockPosX]) 
	{
		collisionRect.x = 50 * blockPosX;
		collisionRect.y = 50 * (blockPosY + 1);
	}
	else
	{
		collisionRect.x = 0;
		collisionRect.y = 0;
	}
}

void droppedItem::render(int deltaX, int deltaY)
{
	if (item.itemType != ITEM_NULL) 
	{
		SDL_Rect tempRect;
		tempRect = mCollider;
		tempRect.x += deltaX;
		tempRect.y += deltaY;
		SDL_RenderDrawRect(gRenderer, &tempRect);
		SDL_Rect tempRect2;
		tempRect = collisionRect;
		tempRect.x += deltaX;
		tempRect.y += deltaY;
		SDL_RenderDrawRect(gRenderer, &tempRect);
		//todo: change the texture
	}
}
/*
void droppedItem::render(int camX, int camY)
{
	//Show the dot relative to the camera
	slime_standing_texture.render(posX - camX, posY - camY);
	slime_walking_texture.render(posX - camX, posY - camY);
}
*/

