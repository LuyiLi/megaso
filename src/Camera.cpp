#include "pch.h"
#include "Camera.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>


Camera::Camera()
{
}

int Camera::countCompensateX(int screenWidth, int posX)
{
	int x = screenWidth / 2 - 35 - posX;
	return x;
}

int Camera::countCompensateY(int screenHight, int posY)
{
	int y = screenHight / 2 - 25 - posY;
	return y;
}

Camera::~Camera()
{
}
