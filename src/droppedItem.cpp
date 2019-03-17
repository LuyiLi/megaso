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
	collisionRect.h = (33);
	collisionRect.w = (33);
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

		//Stop the dropped item If collided
		if (checkCollision())
		{
			//Move back
			mCollider.y -= mVelY;
			mVelY = 0;
		}
		else
		{
			if (abs(mVelY) < 25)
				mVelY += g;
		}
		//update the position of the droppeditem
		if (blockPosY != mCollider.y / (33) || blockPosX != mCollider.x / (33))
		{
			blockPosX = mCollider.x / (33);
			blockPosY = mCollider.y / (33);
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
//update the collision block of the droppeditem
void droppedItem::updateCollisionBox()
{
	if (mainMap.mapData[blockPosY + 1][blockPosX]) 
	{
		collisionRect.x = (33) * blockPosX;
		collisionRect.y = (33) * (blockPosY + 1);
	}
	else
	{
		collisionRect.x = 0;
		collisionRect.y = 0;
	}
}
//render the droppeditem picture
void droppedItem::render(int deltaX, int deltaY)
{
	if (item.itemType != ITEM_NULL) 
	{
		SDL_Rect tempRect;
		tempRect = mCollider;
		tempRect.x += deltaX;
		tempRect.y += deltaY;
		SDL_Rect tempRect2;
		tempRect2 = collisionRect;
		tempRect2.x += deltaX;
		tempRect2.y += deltaY;
		
		//todo: change the texture
		SDL_Rect* currentDroppedItemClip = &mainMap.newMap_clips[item.ID];
		mainMap.newMap_texture.render(mCollider.x+deltaX,mCollider.y+deltaY, currentDroppedItemClip, 0, NULL, SDL_FLIP_NONE, 6);
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);
		//SDL_RenderDrawRect(gRenderer, &tempRect2);
	}
}
