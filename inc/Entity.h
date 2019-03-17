#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "droppedItem.h"
#include "item.h"

class Enemy;
class Player;

enum AttackMode
{
	ATTACKMODE_NONE=0,
	ATTACKMODE_PREPARE,
	ATTACKMODE_ATTACKING,
	ATTACKMODE_FINISH
};

extern const int g;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

enum EnemyAI
{
	AI_WARRIOR,
	AI_PANGOLIN
};

class Enemy
{

public:
	//The dimensions of the Enemy
	int Enemy_WIDTH = 170;
	int Enemy_HEIGHT = 152;

	//The position of the Enemy
	int posX;
	int posY;

	int blockPosX;
	int blockPosY;

	//the whole blood(maximum)
	int healthLimit;
	//the current blood
	int healthPoint;
	//whether it is hitted
	int hitFlag;
	// whether reimu rubo hits the enemy or whether the enemy runs away
	bool canBeHit;

	bool canJump;
	//whether the enemy runs away
	bool canBeKnockedBack;
	//the enemys attacking degree
	int damage;
	int acceleration;

	//which status the enemy is
	AttackMode attackMode;

	SDL_Rect rectArray[36];

	//Maximum axis velocity of the Enemy
	int Enemy_VEL = 5;

	//Initializes the variables
	Enemy();

	//Takes key presses and adjusts the Enemy's velocity

	//Moves the Enemy and checks collision
	void move();
	void getHit(Player *);

	//the enemy's moving image 
	void moveAction(int, int);
	bool loadTexture();

	//check the collision of the boundary
	bool checkCollision();
	//update the items collision box 
	void updateCollisionBox();

	//Shows the Enemy on the screen
	void changeEnemyBehavior();

	void render(int camX, int camY);

	int getPosX();
	int getPosY();

	int getVelX();
	int getVelY();

	EnemyAI AI;
	//The velocity of the Enemy
	int mVelX, mVelY;

	//Enemy's collision box
	SDL_Rect mCollider;

	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;

	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;
};

class Player
{
public:
	//The dimensions of the Player
	static const int Player_WIDTH = 170;
	static const int Player_HEIGHT = 152;

	//The position of the Player
	int posX;
	int posY;

	SDL_Point weaponCollisionPoints[5];

	int blockPosX;
	int blockPosY;

	int healthLimit;
	int healthPoint;

	int hitFlag;
	bool canBeHit;
	bool canJump;

	//current item reimu rubo hands in
	Item currentItem;
	//the weapon's attacking angle 
	double currentAngle;
	bool isUsing;
	int weaponState;
	int acceleration;

	SDL_Rect rectArray[16];

	//Maximum axis velocity of the Player
	static const int Player_VEL = 7;

	//Initializes the variables
	Player();

	//Takes key presses and adjusts the Player's velocity
	void handleEvent(SDL_Event& e);
	void getKilled();
	//Moves the Player and checks collision
	void move();

	void moveAction(int, int);
	bool loadTexture();

	bool checkCollision();
	void pickUpItem(droppedItem *);
	void updateCollisionBox();


	void getHit(Enemy*);
	//The velocity of the Player
	int mVelX, mVelY;

	//Player's collision box
	SDL_Rect mCollider;

	
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;

	SDL_Rect slime_stand_side_clips[6];
	LTexture slime_standing_side_texture;
};
