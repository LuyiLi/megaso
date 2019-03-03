#pragma once
//Åö×²µãÀà
extern int pos_x;
extern int pos_y;
extern const int g;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
class Dot
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 30;
	static const int DOT_HEIGHT = 30;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 5;

	//Initializes the variables
	Dot();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and checks collision
	void move(SDL_Rect& wall);

	//Shows the dot on the screen
	void render();

	//The velocity of the dot
	int mVelX, mVelY;

	//Dot's collision box
	SDL_Rect mCollider;
};

