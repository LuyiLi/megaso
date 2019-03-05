#pragma once
#include "pch.h"
extern int pos_x;
extern int pos_y;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
class Camera
{
public:
	Camera();
	int countCompensateX(int, int);
	int countCompensateY(int, int);
	~Camera();

};

