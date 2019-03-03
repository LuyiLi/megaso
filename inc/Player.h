#pragma once
//Åö×²µãÀà
extern int pos_x;
extern int pos_y;
extern const int g;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
class Player
{
public:
	//The dimensions of the Player
	static const int Player_WIDTH = 190;
	static const int Player_HEIGHT = 190;

	//Maximum axis velocity of the Player
	static const int Player_VEL = 5;

	//Initializes the variables
	Player();

	//Takes key presses and adjusts the Player's velocity
	void handleEvent(SDL_Event& e);

	//Moves the Player and checks collision
	void move(SDL_Rect& wall);

	//Shows the Player on the screen
	void render();

	//The velocity of the Player
	int mVelX, mVelY;

	//Player's collision box
	SDL_Rect mCollider;
};

