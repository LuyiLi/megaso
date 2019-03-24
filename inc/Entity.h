#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "droppedItem.h"
#include "item.h"
#include "EnemyData.h"
#include "Projectile.h"
class Enemy;
class Player;

extern const float g;
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
	int accelerationY;
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
	void getHitProjectile(Projectile *);
	void moveAction(int, int);
	void create(int x, int y, EnemyData *data);
	bool checkCollision();
	void getKilled();
	void updateCollisionBox();
	void updateMoveAction();
	//Shows the Enemy on the screen
	void changeEnemyBehavior();

	//The velocity of the Enemy
	float mVelX, mVelY;

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

	int haveLight;

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
	float acceleration;
	int frame_walk = 0;
	int frame_stand = 0;
	SDL_Rect rectArray[20];

	//Maximum axis velocity of the Player
	static const int Player_VEL = 5;

	//Initializes the variables
	Player();

	//Takes key presses and adjusts the Player's velocity
	void handleEvent(SDL_Event& e);
	void getKilled();
	//Moves the Player and checks collision
	void move();
	void updateMoveAction();
	void moveAction(int deltaX, int deltaY, int posX, int posY);
	bool loadTexture();

	bool checkCollision();
	void pickUpItem(droppedItem *);
	void updateCollisionBox();
	void getHit(Enemy*);
	//The velocity of the Player
	float mVelX, mVelY;

	//Player's collision box
	SDL_Rect mCollider;

	/*վ��״̬ͼ����Ƭ*/
	//const int standing_frames = 6;
	SDL_Rect slime_stand_clips[6];
	LTexture slime_standing_texture;
	LTexture slime_standing_texture_blue;
	LTexture slime_standing_texture_green;
	LTexture slime_standing_texture_red;
	/*����״̬ͼ����Ƭ*/
	//const int walking_frames = 4;
	SDL_Rect slime_walk_clips[4];
	LTexture slime_walking_texture;
	LTexture slime_walking_texture_blue;
	LTexture slime_walking_texture_green;
	LTexture slime_walking_texture_red;

	SDL_Rect slime_stand_side_clips[6];
	LTexture slime_standing_side_texture;
	LTexture slime_standing_side_texture_blue;
	LTexture slime_standing_side_texture_green;
	LTexture slime_standing_side_texture_red;
};
