#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Entity.h"
#include "Camera.h"
#include "settings.h"
#include "SavingControl.h"
#include "Map.h"
#include "item.h"
#include "ItemList.h"
#include "droppedItem.h"
#include "pocket.h"
#include "global.h"
#include <SDL_ttf.h>
#include <cmath>
#include "InitEnemyList.h"
#include "Projectile.h"
#include <thread>
/*
				   _ooOoo_
				  o8888888o
				  88" . "88
				  (| -_- |)
				  O\  =  /O
			   ____/`---'\____
			 .'  \\|     |//  `.
			/  \\|||  :  |||//  \
		   /  _||||| -:- |||||-  \
		   |   | \\\  -  /// |   |
		   | \_|  ''\---/''  |   |
		   \  .-\__  `-`  ___/-. /
		 ___`. .'  /--.--\  `. . __
	  ."" '<  `.___\_<|>_/___.'  >'"".
	 | | :  `- \`.;`\ _ /`;.`/ - ` : | |
	 \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
				   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			Should there be no BUG in the program
			In the blessing of the Budda
*/

Item itemList[500];
EnemyData enemyDataList[10];
droppedItem droppedItemList[200];
Enemy enemyList[20];

//the type of player
Player player;
//the class of camera;to present the picture into screen 
Camera cam;
//to save the target file for reading and writing
SavingControl savingControler;
TTF_Font *gFont = NULL;
Map mainMap;
pocket mainPocket;
Projectile projectile;
//Rendered texture

//a structure presenting renderer state
SDL_Renderer* gRenderer = NULL;
//a type used to identify a window
SDL_Window* gWindow = NULL;
//all the clips and their texture
SDL_Rect heart_clips[8];
LTexture heart_texture;

SDL_Rect* crackClip1;
SDL_Rect* crackClip2;
SDL_Rect* crackClip3;

SDL_Rect magic_clips[8];
LTexture magic_texture;

SDL_Rect crack_clips[3];
LTexture crack_texture;

SDL_Rect dead_clips[1];
LTexture dead_texture;

SDL_Rect tool_clips[23];
LTexture tool_texture;

SDL_Rect weapon_clips[23];
LTexture weapon_texture;

SDL_Rect hp_clips[2];
LTexture hp_texture;

SDL_Rect mp_clips[2];
LTexture mp_texture;

SDL_Rect title_clips;
LTexture title1_texture;
LTexture title2_texture;
LTexture megaso_texture;

SDL_TimerID backgroundTimer;

LTexture gPlayerTexture;

SDL_Point centralPoint[4];
int direction = 0;
int worldTime = 0;
bool quit = false;
bool init();
bool loadMedia();
void close();
int haveTorch = 0;
bool projectileReady = true;
int projectileFlag = 0;
int pocketNumber = 1;
int prevPocketNumber = 1;
int breakTime = 2000;
int startTime = 0;
int target[23] = {82500,3000,100,0};
int bgIsChanging=0;
int mouseX, mouseY, mouseState;
int crackFlag;
int blockMouseX, blockMouseY;
int absMouseX;
int absMouseY;//mouseState = 0/1/2/4  NULL/left/middle/right
int isTakenUp = 0;
int IDWithMouse = 0, numWithMouse = 0;
int heartFrame = 0;
int magicFrame = 0;
int lightBlockPosX;
int lightBlockPosY;
bool renderFlag = true;
bool calculateLightFinishFlag = false;
GroundBiomeTypes currentBiome = mainMap.currentBiome(player.blockPosX);
GroundBiomeTypes targetState = mainMap.currentBiome(player.blockPosX);
int gameStarted = 0;
double angleForBlock = 0;

bool init()
{
	mainMap.preAlpha = 0;
	mainMap.targetAlpha = 255;
	//Check if map.txt exist, generate one if not
	if (mainMap.checkIfExist()&&mainMap.checkIfWallExist()&&mainMap.checkIfBiomeExist())
	{
		mainMap.mapRead();
		mainMap.wallRead();
		mainMap.biomeRead();
	}
	else
	{
		mainMap.generateMap();
		mainMap.mapWrite();
		mainMap.wallWrite();
		mainMap.biomeWrite();
		mainMap.mapRead();
		mainMap.wallRead();
		mainMap.biomeRead();
	}

	//init the ItemList
	initItemList();

	//init the enemyDataList
	initEnemyDataList();
	//Check if pocket.txt exist, generate one if not
	if (mainPocket.checkIfExist())
	{
		mainPocket.pocketRead();
	}

	else
	{
		mainPocket.pocketGenerate();
		mainPocket.pocketWrite(mainPocket.pocketData);
		mainPocket.pocketRead();
	}

	//Initialization flag
	bool success = true;

	//Read position from saving.txt
	savingControler.fileRead(target);
	player.mCollider.x = target[0];
	player.mCollider.y = target[1];
	player.healthPoint = target[2];
	mainPocket.accessories = target[3];
	worldTime = target[3];
	//Initialize currentItem
	player.currentItem = itemList[mainPocket.pocketData[pocketNumber - 1][0]];
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("REIMU-RUBO----Bulid Your Own Story", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	//enemyList[0].create(500, 100, &enemyDataList[1]);
	//enemyList[1].create(500, 200, &enemyDataList[1]);
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("comic.ttf", 32);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	
	if (dead_texture.loadFromFile("images/dead.png"))
	{
		dead_clips[0].x = 0;
		dead_clips[0].y = 0;
		dead_clips[0].w = 1800;
		dead_clips[0].h = 1200;
	}



	if (heart_texture.loadFromFile("images/heart.png"))
	{
		for (int i = 0; i < 8; i++)
		{
			heart_clips[i].x = i * 100;
			heart_clips[i].y = 0;
			heart_clips[i].w = 100;
			heart_clips[i].h = 100;
		}
		
	}

	if (magic_texture.loadFromFile("images/magic.png"))
	{
		for (int i = 0; i < 8; i++)
		{
			magic_clips[i].x = i * 100;
			magic_clips[i].y = 0;
			magic_clips[i].w = 100;
			magic_clips[i].h = 100;
		}

	}

	if (tool_texture.loadFromFile("images/tools.png"))
	{
		for (int i = 0; i < 23; i++)
		{
			tool_clips[i].x = 100 * i;
			tool_clips[i].y = 0;
			tool_clips[i].w = 100;
			tool_clips[i].h = 100;
		}
	}

	if (weapon_texture.loadFromFile("images/weapons.png"))
	{
		for (int i = 0; i < 23; i++)
		{
			weapon_clips[i].x = 100 * i;
			weapon_clips[i].y = 0;
			weapon_clips[i].w = 100;
			weapon_clips[i].h = 100;
		}
	}

	//Load crack effect
	if (crack_texture.loadFromFile("images/crack.png"))
	{
		for (int i = 0; i < 3; i++)
		{
			crack_clips[i].x = i*100;
			crack_clips[i].y = 0;
			crack_clips[i].w = 100;
			crack_clips[i].h = 100;
		}
	}

	if (hp_texture.loadFromFile("images/healthPoint.png"))
	{
		for (int i = 0; i < 2; i++)
		{
			hp_clips[i].x = i * 100;
			hp_clips[i].y = 0;
			hp_clips[i].w = 100;
			hp_clips[i].h = 100;
		}
	}

	if (mp_texture.loadFromFile("images/magicPoint.png"))
	{
		for (int i = 0; i < 2; i++)
		{
			mp_clips[i].x = i * 100;
			mp_clips[i].y = 0;
			mp_clips[i].w = 100;
			mp_clips[i].h = 100;
		}
	}

	if (title1_texture.loadFromFile("images/title1.png") && title2_texture.loadFromFile("images/title2.png") && megaso_texture.loadFromFile("images/megaso.png"))
	{
		title_clips.x = 0;
		title_clips.y = 0;
		title_clips.w = 900;
		title_clips.h = 600;
	}
	else
	{
		printf("Failed to load media\n");
	}
	for (int i = 0; i < 10; i++)
	{
		if (enemyDataList[i].ID)
			if (!enemyDataList[i].loadTexture())
				success = false;
	}
	if (!player.loadTexture() || !mainMap.loadTexture() || !projectile.loadTexture())
	{
		success = false;
	}
	crackClip1 = &crack_clips[0];
	crackClip2 = &crack_clips[1];
	crackClip3 = &crack_clips[2];
	mainPocket.loadTexture();

	return success;
}

void close()
{
	//Saving last position into saving.txt
	int data[23] = {0};
	data[0] = player.mCollider.x;
	data[1] = player.mCollider.y;
	data[2] = player.healthPoint;
	data[3] = mainPocket.accessories;
	savingControler.fileWrite(data);

	//Saving last map condition into map.txt
	mainMap.mapWrite();
	//Free loaded images
	mainPocket.pocketWrite(mainPocket.pocketData);
	//Quit SDL subsystems
	exit(0);
	
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

Uint32 renderBgChangeCallback(Uint32 interval, void* param)
{
	if (!mainMap.renderBgChange(targetState))
	{
		bgIsChanging = 1;
		SDL_TimerID backgroundTimer = SDL_AddTimer(20, renderBgChangeCallback, (void*)mouseState);
	}
	else
	{
		GroundBiomeTypes temp;
		temp = targetState;
		targetState = currentBiome;
		currentBiome = temp;
		bgIsChanging = 0;
	}
	return 0;
}

void foo()
{
	renderFlag = false;
	int posX = player.posX;
	int posY = player.posY;
	int blockPosX = player.blockPosX;
	int blockPosY = player.blockPosY;
	lightBlockPosX = blockPosX;
	lightBlockPosY = blockPosY;
	int deltaX = cam.countCompensateX(SCREEN_WIDTH, player.posX);
	int deltaY = cam.countCompensateY(SCREEN_HEIGHT, player.posY);
	SDL_Rect* currentHeartClip;
	SDL_Rect* currentMagicClip;

	while (1)
	{
		if (renderFlag)
		{

			deltaX = cam.countCompensateX(SCREEN_WIDTH, player.posX);
			deltaY = cam.countCompensateY(SCREEN_HEIGHT, player.posY);

			//renderFlag = false;
			posX = player.posX;
			posY = player.posY;
			blockPosX = player.blockPosX;
			blockPosY = player.blockPosY;
			lightBlockPosX = blockPosX;
			lightBlockPosY = blockPosY;
			calculateLightFinishFlag = false;

			mainMap.renderBg(currentBiome, targetState);
			mainMap.renderWall(deltaX, deltaY, blockPosX, blockPosY);
			if (crackFlag && mainMap.wallData[blockMouseY][blockMouseX] && abs(blockMouseX - blockPosX) + abs(blockMouseY - blockPosY) <= 4 && mainPocket.pocketData[0][pocketNumber - 1] == 305)
			{
				switch (crackFlag)
				{
				case 1:crack_texture.render(blockMouseX * 33 + deltaX, blockMouseY * 33 + deltaY, crackClip1, 0, NULL, SDL_FLIP_NONE, 3); break;
				case 2:crack_texture.render(blockMouseX * 33 + deltaX, blockMouseY * 33 + deltaY, crackClip2, 0, NULL, SDL_FLIP_NONE, 3); break;
				case 3:crack_texture.render(blockMouseX * 33 + deltaX, blockMouseY * 33 + deltaY, crackClip3, 0, NULL, SDL_FLIP_NONE, 3); break;
				}
			}
			mainMap.render(deltaX, deltaY, blockPosX, blockPosY);

			//3 Clips for crack effects

			if (crackFlag && mainMap.mapData[blockMouseY][blockMouseX] && abs(blockMouseX - blockPosX) + abs(blockMouseY - blockPosY) <= 4
				&& (!mainMap.mapData[blockMouseY + 1][blockMouseX] || !mainMap.mapData[blockMouseY][blockMouseX + 1] || !mainMap.mapData[blockMouseY - 1][blockMouseX] || !mainMap.mapData[blockMouseY][blockMouseX - 1])
				&& mainPocket.pocketData[0][pocketNumber - 1] != 305 && mainPocket.pocketData[0][pocketNumber - 1] != 306 && mainPocket.pocketData[0][pocketNumber - 1] != 307)
			{
				switch (crackFlag)
				{
				case 1:crack_texture.render(blockMouseX*(33) + deltaX, blockMouseY*(33) + deltaY, crackClip1, 0, NULL, SDL_FLIP_NONE, 3); break;
				case 2:crack_texture.render(blockMouseX*(33) + deltaX, blockMouseY*(33) + deltaY, crackClip2, 0, NULL, SDL_FLIP_NONE, 3); break;
				case 3:crack_texture.render(blockMouseX*(33) + deltaX, blockMouseY*(33) + deltaY, crackClip3, 0, NULL, SDL_FLIP_NONE, 3); break;
				}
			}

			for (int i = 0; i < 200; i++)
				droppedItemList[i].render(deltaX, deltaY);

			for (int i = 0; i < 20; i++)
				enemyList[i].moveAction(deltaX, deltaY);


			mainPocket.mainPocketRender();

			double percentage = (double)player.healthPoint / (double)player.healthLimit;
			hp_texture.renderWithScale(SCREEN_WIDTH / 2 + 160, 25, &heart_clips[0], 0, NULL, SDL_FLIP_NONE, 0.4, 2, percentage);

			hp_texture.render(int(SCREEN_WIDTH / 2 + 160 + (250 * percentage)), 25, &heart_clips[1], 0, NULL, SDL_FLIP_NONE, 2);

			currentHeartClip = &heart_clips[heartFrame / 8];
			heart_texture.render(SCREEN_WIDTH / 2 + 130, 25, currentHeartClip, 0, NULL, SDL_FLIP_NONE, 2);

			double mpPercentage = (double)player.magicPoint / (double)player.magicLimit;
			mp_texture.renderWithScale(SCREEN_WIDTH / 2 + 160, 75, &magic_clips[0], 0, NULL, SDL_FLIP_NONE, 0.4, 2, mpPercentage);

			mp_texture.render(int(SCREEN_WIDTH / 2 + 160 + (250 * mpPercentage)), 75, &magic_clips[1], 0, NULL, SDL_FLIP_NONE, 2);

			currentMagicClip = &magic_clips[magicFrame / 8];
			magic_texture.render(SCREEN_WIDTH / 2 + 130, 75, currentMagicClip, 0, NULL, SDL_FLIP_NONE, 2);
			

			centralPoint[0].x = 25 * 2 / 3;
			centralPoint[0].y = 75 * 2 / 3;
			centralPoint[1].x = 75 * 2 / 3;
			centralPoint[1].y = 75 * 2 / 3;
			centralPoint[2].x = 0;
			centralPoint[2].y = 100 * 2 / 3;
			centralPoint[3].x = 100 * 2 / 3;
			centralPoint[3].y = 100 * 2 / 3;

			int R = mainMap.lightBlock[21][21], G = mainMap.lightBlock[21][21], B = mainMap.lightBlock[21][21];
			if (mainPocket.pocketData[0][pocketNumber - 1] > 0 && mainPocket.pocketData[0][pocketNumber - 1] <= 100)
			{

				mainMap.newMap_texture.setColor(R, G, B);
				if (player.acceleration > 0)
				{
					mainMap.newMap_texture.render(SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], angleForBlock - 20, &centralPoint[0], SDL_FLIP_NONE, 4);
				}
				else if (player.acceleration < 0)
				{
					mainMap.newMap_texture.render(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], -angleForBlock + 20, &centralPoint[0], SDL_FLIP_HORIZONTAL, 4);
				}
				else
				{
					int mouseX, mouseY, mouseState;
					mouseState = SDL_GetMouseState(&mouseX, &mouseY);


					if (mouseX > SCREEN_WIDTH / 2)
					{
						player.weaponState = 1;
						mainMap.newMap_texture.render(SCREEN_WIDTH / 2 + 15, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], angleForBlock - 20, &centralPoint[0], SDL_FLIP_NONE, 4);
					}
					else
					{
						player.weaponState = 2;
						mainMap.newMap_texture.render(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], -angleForBlock + 20, &centralPoint[0], SDL_FLIP_HORIZONTAL, 4);
					}
				}
				mainMap.newMap_texture.setColor(255, 255, 255);
			}
			else if (mainPocket.pocketData[0][pocketNumber - 1] > 300 && mainPocket.pocketData[0][pocketNumber - 1] <= 400)
			{
				tool_texture.setColor(R, G, B);
				if (player.acceleration > 0)
				{
					tool_texture.render(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 - 45, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1] - 300], player.currentAngle, &centralPoint[0], SDL_FLIP_NONE, 1.5);
					direction = 1;
				}
				else if (player.acceleration < 0)
				{
					tool_texture.render(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 45, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1] - 300], -player.currentAngle, &centralPoint[1], SDL_FLIP_HORIZONTAL, 1.5);
					direction = 0;
				}
				else
				{
					int mouseX, mouseY, mouseState;
					mouseState = SDL_GetMouseState(&mouseX, &mouseY);
					if (mouseX > SCREEN_WIDTH / 2)
					{
						direction = 1;
						player.weaponState = 1;
						tool_texture.render(SCREEN_WIDTH / 2 + 15, SCREEN_HEIGHT / 2 - 45, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1] - 300], player.currentAngle, &centralPoint[0], SDL_FLIP_NONE, 1.5);
					}
					else
					{
						tool_texture.render(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 45, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1] - 300], -player.currentAngle, &centralPoint[1], SDL_FLIP_HORIZONTAL, 1.5);
						direction = 0;
						player.weaponState = 2;
					}
				}
				tool_texture.setColor(255, 255, 255);
			}
			else if (mainPocket.pocketData[0][pocketNumber - 1] > 400 && mainPocket.pocketData[0][pocketNumber - 1] <= 500)
			{
				weapon_texture.setColor(R, G, B);
				if (player.acceleration > 0)
				{
					weapon_texture.render(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 - 55, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], player.currentAngle, &centralPoint[2], SDL_FLIP_NONE, 1.5);
					direction = 1;
				}
				else if (player.acceleration < 0)
				{
					weapon_texture.render(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 55, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], -player.currentAngle, &centralPoint[3], SDL_FLIP_HORIZONTAL, 1.5);
					direction = 0;
				}
				else
				{
					int mouseX, mouseY, mouseState;
					mouseState = SDL_GetMouseState(&mouseX, &mouseY);


					if (mouseX > SCREEN_WIDTH / 2)
					{
						weapon_texture.render(SCREEN_WIDTH / 2 + 15, SCREEN_HEIGHT / 2 - 55, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], player.currentAngle, &centralPoint[2], SDL_FLIP_NONE, 1.5);
						direction = 1;
						player.weaponState = 1;
					}
					else
					{
						weapon_texture.render(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 55, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], -player.currentAngle, &centralPoint[3], SDL_FLIP_HORIZONTAL, 1.5);
						direction = 0;
						player.weaponState = 2;
					}
				}
				weapon_texture.setColor(255, 255, 255);

			}
			else
			{
				player.weaponState = 0;
			}

			projectile.moveAction(deltaX, deltaY);
			player.moveAction(deltaX, deltaY, posX, posY);
			if (player.isDead)
			{
				dead_texture.render(0, 0, dead_clips, 0, NULL, SDL_FLIP_NONE, 2);
				SDL_RenderPresent(gRenderer);
				SDL_Delay(5000);
				player.isDead = 0;
			}

			SDL_RenderPresent(gRenderer);
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			//Clear the render for next use
			SDL_RenderClear(gRenderer);
			//SDL_Delay(15);
		}
	}
}


Uint32 movementCallback(Uint32 interval, void* param)
{
	for (int i = 0; i < 20; i++)
	{
		enemyList[i].updateMoveAction();
	}
	//update the frame of hp and mp
	++heartFrame;
	if (heartFrame / 8 >= 8)
	{
		heartFrame = 0;
	}
	++magicFrame;
	if (magicFrame / 8 >= 8)
	{
		magicFrame = 0;
	}
	//update the frame of player
	player.updateMoveAction();
	//Reimu-Rubo moves
	player.move();
	mainMap.updateScroll();
	projectile.move();
	if (!projectileReady)
	{
		projectileFlag++;
		if (projectileFlag > 10)
		{
			projectileReady = true;
			projectileFlag = 0;
		}
	}
	for (int i = 0; i < 20; i++)
	{
		enemyList[i].move();
		player.getHit(&enemyList[i]);
	}
	if (mainPocket.accessories > 0)
	{
		player.haveLight = 1;
	}
	else if (mainPocket.pocketData[0][pocketNumber - 1] == 307)
	{
		player.haveLight = 1;
	}
	else if (pocketNumber == 1)
	{
		if (mainPocket.pocketData[0][1] == 307)
		{
			player.haveLight = 1;
		}
		else
		{
			player.haveLight = 0;
		}
	}
	else if (pocketNumber > 1)
	{
		if (mainPocket.pocketData[0][pocketNumber] == 307 || mainPocket.pocketData[0][pocketNumber - 2] == 307)
		{
			player.haveLight = 1;
		}
		else
		{
			player.haveLight = 0;
		}
	}
	else
	{
		player.haveLight = 0;
	}
	targetState = mainMap.currentBiome(player.blockPosX);
	if (targetState != currentBiome && !bgIsChanging)
	{
		backgroundTimer = SDL_AddTimer(20, renderBgChangeCallback, (void*)mouseState);
	}
	for (int i = 0; i < 200; i++)
	{
		player.pickUpItem(&droppedItemList[i]);
		droppedItemList[i].move();
	}

	for (int i = 0; i < 20; i++)
	{
		enemyList[i].getHit(&player);
		enemyList[i].getHitProjectile(&projectile);
	}
	renderFlag = true;
	SDL_TimerID movementTimer = SDL_AddTimer(16, movementCallback, (void*)"a");
	return 0;
}

Uint32 mouseTimerCallback(Uint32 interval, void* param)
{
	static int prevMouseState;
	static int prevBlockMouseX, prevBlockMouseY;
	static int flag = 0;

	//Get mouse state
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	//Translate mouse position into block position
	absMouseX = mouseX - cam.countCompensateX(SCREEN_WIDTH, player.posX);
	absMouseY = mouseY - cam.countCompensateY(SCREEN_HEIGHT, player.posY);
	blockMouseX = absMouseX / (33);
	blockMouseY = absMouseY / (33);
	//If the same button is still being pressed
	if (mouseState != prevMouseState)
	{
		player.currentAngle = 0;
	}
	if (mouseState == prevMouseState)
	{
		//If pressed the left buttom
		if (mouseState == 1)
		{
			player.isUsing = true;
			if (player.currentAngle < 60)
			{
				player.currentAngle += 6;
			}
			else
			{
				player.currentAngle = -60;
			}
			//If the mouse is on the same place
			if (blockMouseX == prevBlockMouseX && blockMouseY == prevBlockMouseY && mainPocket.pocketData[0][pocketNumber - 1] > 300 && mainPocket.pocketData[0][pocketNumber - 1] <= 400 && mainPocket.pocketData[0][pocketNumber - 1] != 305 && mainPocket.pocketData[0][pocketNumber - 1] != 306 && mainPocket.pocketData[0][pocketNumber - 1] != 307)
			{
				//Break the block if time is enough
				if (flag == player.currentItem.breakTime)
				{
					//Break a block in the map
					mainMap.breakBlock(blockMouseX, blockMouseY);
					//Update the CollisionBox of the block
					player.updateCollisionBox();
					//Add the block into pocket
					mainPocket.pocketUpdate();

					//update dropped item's collision box
					for (int i = 0; i < 200; i++)
						droppedItemList[i].updateCollisionBox();

					//Reset flags to 0
					flag = 0;
					crackFlag = 0;

					//Timer set
					SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
					return 0;
				}

				// block is not broken if time is not enough
				crackFlag = 4 * flag / player.currentItem.breakTime;
				flag++;

				//Timer set
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
			if (blockMouseX == prevBlockMouseX && blockMouseY == prevBlockMouseY && mainPocket.pocketData[0][pocketNumber - 1] == 305)
			{
				//Break the block if time is enough
				if (flag == 40)
				{
					//Break a block in the map
					mainMap.breakWall(blockMouseX, blockMouseY);
					//Update the CollisionBox of the block
					player.updateCollisionBox();
					//Add the block into pocket
					mainPocket.pocketUpdate();

					//update dropped item's collision box
					for (int i = 0; i < 200; i++)
						droppedItemList[i].updateCollisionBox();

					//Reset flags to 0
					flag = 0;
					crackFlag = 0;

					//Timer set
					SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
					return 0;
				}
				// block is not broken if time is not enough
				crackFlag = flag / 10;
				flag++;
				//Timer set
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
		}

		//If the mouse moved to another block
		flag = 0;
		crackFlag = 0;
		prevBlockMouseX = blockMouseX;
		prevBlockMouseY = blockMouseY;

		//Timer set
		SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
		return 0;
		}
	
		else if (mouseState == 4 && mainPocket.pocketData[0][pocketNumber - 1] <= 100)
		{
			//keep putting things on the floor
			if (mainPocket.pocketData[1][pocketNumber - 1])
			{
				if (angleForBlock < 70)
				{
					angleForBlock += 4;
				}
				else
				{
					angleForBlock = 0;
				}

				//mainMap.putBlock(blockMouseX, blockMouseY, pocketNumber);
				mainMap.putBlock(blockMouseX, blockMouseY, mainPocket.pocketData[0][pocketNumber - 1]);

				player.updateCollisionBox();
				prevBlockMouseX = blockMouseX;
				prevBlockMouseY = blockMouseY;
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
			else
			{
				//TImer set
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
		}
		else if (mouseState == 4 && mainPocket.pocketData[0][pocketNumber - 1] <= 200)
		{
			//keep putting things on the floor
			if (mainPocket.pocketData[1][pocketNumber - 1])
			{
				if (angleForBlock < 70)
				{
					angleForBlock += 4;
				}
				else
				{
					angleForBlock = 0;
				}

				//mainMap.putBlock(blockMouseX, blockMouseY, pocketNumber);
				mainMap.putWall (blockMouseX, blockMouseY, mainPocket.pocketData[0][pocketNumber - 1]);

				player.updateCollisionBox();
				prevBlockMouseX = blockMouseX;
				prevBlockMouseY = blockMouseY;
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
			else
			{
				//TImer set
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
		}
		else if (!mouseState)
		{
			angleForBlock = 0;
			crackFlag = 0;
			player.isUsing = false;
			return 0;
		}
		
	
	flag = 0;
	crackFlag = 0;
	prevMouseState = mouseState;
	SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
	return 0;

	//SDL_TimerID mouseTimer = SDL_AddTimer(10, mouseTimerCallback, (void*)mouseState);
}

Uint32 mainMapUpdate(Uint32 interval, void* param)
{
	int generationTries = (worldTime > 600 ? 2 : 1);
	int blockX, blockY;
	static int generateFlag = 500;
	bool generationFlag;
	if (worldTime >= 1200)
		worldTime = 0;
	if (generateFlag <= 0)
	{
		generateFlag = random01() * 300 + 200;
		for (int i = 0; i < generationTries; i++)
		{
			blockX = (random01() * 20 + 20) * (random01() * 2 < 1 ? 1 : -1) + player.blockPosX;
			blockY = (random01() * 20) * (random01() * 2 < 1 ? 1 : -1) + player.blockPosY;
			generationFlag = true;
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
				{
					if (mainMap.mapData[blockY + j][blockX + k])
						generationFlag = false;
				}
			if (generationFlag || blockY > 300)
				for (int i = 0; i < 10; i++)
				{
					if (!(enemyList[i].isAlive))
					{
						enemyList[i].create(33 * blockX + 10, 33 * blockY + 20, &enemyDataList[mainMap.calculateEnemyGenerationRate(blockX, blockY)]);
						break;
					}
				}
		}
	}
	else
		worldTime > 600 ? generateFlag -= 2 : generateFlag--;
	mainMap.countBgColor();
	mainMap.countFrontBgColor();
	worldTime++;
	SDL_TimerID mainTimer = SDL_AddTimer(250, mainMapUpdate, (void*)"a");
	return 0;
}

int main(int argc, char* args[])
{
	if (!init());
	else
	{
		if (!loadMedia());
		else
		{
			megaso_texture.render(0, 0, &title_clips, 0, NULL, SDL_FLIP_NONE, 1);
			SDL_RenderPresent(gRenderer);
			SDL_Delay(1500);
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			//Clear the render for next use
			SDL_RenderClear(gRenderer);
			int time = 0;
			while (gameStarted == 0)
			{
				SDL_Event e;
				time++;
				if (time <= 50)
				{
					title1_texture.render(0, 0, &title_clips, 0, NULL, SDL_FLIP_NONE, 1);
				}
				else
				{
					title2_texture.render(0, 0, &title_clips, 0, NULL, SDL_FLIP_NONE, 1);
				}
				SDL_RenderPresent(gRenderer);
				if (time == 101)
				{
					time = 0;
				}
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
					{
						gameStarted = 1;
					}
				}
			}
			if(gameStarted)
			{
				bool quit = false;
				SDL_Event e;
				try {
					std::thread t1(foo);
					t1.detach();
				}
				catch (std::system_error) {
					SDL_Delay(30);
					std::thread t1(foo);
					t1.detach();
				}
				SDL_TimerID movementTimer = SDL_AddTimer(16, movementCallback, (void*)"a");
				SDL_TimerID mainTimer = SDL_AddTimer(1000, mainMapUpdate, (void*)"a");
				while (!quit)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						
						if (e.type == SDL_KEYDOWN)
						{
							/*
							if (e.key.keysym.sym == SDLK_x)
							{
								for (int i = 0; i < 10; i++)
								{
									if (!(enemyList[i].isAlive))
									{
										enemyList[i].create(player.mCollider.x, player.mCollider.y - 200, &enemyDataList[1]);
										break;
									}
								}
							}
							if (e.key.keysym.sym == SDLK_c)
							{
								for (int i = 0; i < 10; i++)
								{
									if (!(enemyList[i].isAlive))
									{
										enemyList[i].create(player.mCollider.x - 500, player.mCollider.y - 100, &enemyDataList[2]);
										break;
									}
								}
							}
							if (e.key.keysym.sym == SDLK_v)
							{
								for (int i = 0; i < 10; i++)
								{
									if (!(enemyList[i].isAlive))
									{
										enemyList[i].create(player.mCollider.x + 400, player.mCollider.y + 300, &enemyDataList[3]);
										break;
									}
								}
							}
							if (e.key.keysym.sym == SDLK_b)
							{
								for (int i = 0; i < 10; i++)
								{
									if (!(enemyList[i].isAlive))
									{
										enemyList[i].create(player.mCollider.x - 500, player.mCollider.y - 100, &enemyDataList[4]);
										break;
									}
								}
							}
							if (e.key.keysym.sym == SDLK_n)
							{
								for (int i = 0; i < 10; i++)
								{
									if (!(enemyList[i].isAlive))
									{
										enemyList[i].create(player.mCollider.x - 500, player.mCollider.y - 100, &enemyDataList[5]);
										break;
									}
								}
							}
							if (e.key.keysym.sym == SDLK_m)
							{
								for (int i = 0; i < 10; i++)
								{
									if (!(enemyList[i].isAlive))
									{
										enemyList[i].create(player.mCollider.x - 500, player.mCollider.y - 100, &enemyDataList[6]);
										break;
									}
								}
							}
							if (e.key.keysym.sym == SDLK_z)
							{
								for (int i = 0; i < 10; i++)
								{
									if (!(enemyList[i].isAlive))
									{
										enemyList[i].create(player.mCollider.x - 500, player.mCollider.y - 100, &enemyDataList[7]);
										break;
									}
								}
							}
							*/
							if (e.key.keysym.sym == SDLK_f)
							{
								if (player.magicPoint > 12)
								{
									if (projectileReady)
									{
										projectileReady = false;
										SDL_GetMouseState(&mouseX, &mouseY);
										player.magicPoint -= 12;
										projectile.create(player.posX, player.posY, mouseX, mouseY, mainPocket.accessories);
									}

								}
							}
						}

						//User requests quit
						if (e.type == SDL_QUIT)
						{
							quit = true;
							close();
						}
						if (e.type == SDL_MOUSEBUTTONDOWN && !mainPocket.isOpened)
						{
							//Get mouse position
							mouseTimerCallback(0, &mouseState);
						}
						mainPocket.handlePocketEvents(e);
						if (pocketNumber != prevPocketNumber)
						{
							prevPocketNumber = pocketNumber;
							player.currentItem = itemList[mainPocket.pocketData[0][pocketNumber - 1]];
						}
						player.handleEvent(e);
					}
					//SDL_Delay(10);
				}
			}
			return 0;
		}
	}
}
/*********************************************************/