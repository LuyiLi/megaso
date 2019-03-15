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

bool intersect(SDL_Rect a, SDL_Rect b);

bool inRect(SDL_Point, SDL_Rect);
SDL_Texture* loadTexture(std::string path);

float random01();

#endif // !GLOBAL_H