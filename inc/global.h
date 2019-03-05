#pragma once
#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

extern SDL_Renderer* gRenderer;

enum CollisionType
{
	COLLISION_TOP = 1,
	COLLISION_SIDE = 2,
	COLLISION_NONE = 0,
	COLLISION_ERROR = -1
};

bool intersect(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
		return false;
	//If none of the sides from A are outside B
	return true;
}

CollisionType checkCollision(Player *player, SDL_Rect rectB)
{
	//Calculate the sides of rect A
	SDL_Rect rectA = player->mCollider;

	//Check if there is collison from the side
	if (!intersect(rectA, rectB))
		return COLLISION_NONE;
	else
		return COLLISION_SIDE;
}
SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

#endif // !GLOBAL_H