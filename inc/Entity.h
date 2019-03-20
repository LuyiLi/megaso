#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "droppedItem.h"
#include "item.h"
#include "EnemyData.h"

class Enemy;
class Player;

extern const int g;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

enum AttackMode
{
	ATTACKMODE_NONE = 0,
	ATTACKMODE_PREPARE,
	ATTACKMODE_ATTACKING,
	ATTACKMODE_FINISH
};


class Enemy
{

public:
	//The dimensions of the Enemy

	EnemyData *enemyData;
	//The position of the Enemy
	int posX;
	int posY;

	int blockPosX;
	int blockPosY;
	int modeFlag = 0;
	int frame = 0;
	int frameFlag = 0;
	int frame_walk = 0;
	int healthPoint = 100;

	int hitFlag;
	bool canBeHit;
	bool canJump;
	bool canBeKnockedBack;
	int acceleration;
	bool isAlive;
	int angle = 0;
	int Enemy_VEL;
	AttackMode attackMode;
	SDL_Rect rectArray[36];

	//Maximum axis velocity of the Enemy

	//Initializes the variables
	Enemy();

	//Takes key presses and adjusts the Enemy's velocity

	//Moves the Enemy and checks collision
	void move();
	void getHit(Player *);
	void moveAction(int, int);
	void create(int x, int y, EnemyData *data);
	bool checkCollision();
	void getKilled();
	void updateCollisionBox();

	//Shows the Enemy on the screen
	void changeEnemyBehavior();

	//The velocity of the Enemy
	int mVelX, mVelY;

	//Enemy's collision box
	SDL_Rect mCollider;

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

	int previousState;
	int currentBiome;

	int healthLimit;
	int healthPoint;
	int magicLimit;
	int magicPoint;
	int isDead;

	int hitFlag;
	bool canBeHit;
	bool canJump;
	Item currentItem;
	double currentAngle;
	bool isUsing;
	int weaponState;
	int acceleration;

	SDL_Rect rectArray[20];

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

	int getPosX();
	int getPosY();

	int getVelX();
	int getVelY();

	void getHit(Enemy*);
	//The velocity of the Player
	int mVelX, mVelY;

	//Player's collision box
	SDL_Rect mCollider;

	/*վ��״̬ͼ����Ƭ*/
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	/*����״̬ͼ����Ƭ*/
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;

	SDL_Rect slime_stand_side_clips[6];
	LTexture slime_standing_side_texture;
};
