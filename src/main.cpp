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
#include <SDL_ttf.h>
#include <cmath>

Item itemList[500];
droppedItem droppedItemList[200];

Enemy testEnemy;

//�������
Player player;
Camera cam;
SavingControl savingControler;
TTF_Font *gFont = NULL;
Map mainMap;
pocket mainPocket;

//Rendered texture
LTexture gTextTexture1[40];
LTexture gTextTexture2[40];
LTexture gTextTextureOne;
LTexture gTextTextureTwo;
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

SDL_Rect heart_clips[8];
LTexture heart_texture;

SDL_Rect rubbish_clips[1];
LTexture rubbish_texture;

SDL_Rect crack_clips[3];
LTexture crack_texture;

SDL_Rect very_behind_background_clips[1];
LTexture very_behind_background_texture;

SDL_Rect pocketUI_clips[3];
LTexture pocketUI_texture;

SDL_Rect tool_clips[1];
LTexture tool_texture;

SDL_Rect weapon_clips[1];
LTexture weapon_texture;

SDL_Rect hp_clips[2];
LTexture hp_texture;

LTexture gPlayerTexture;

bool quit = false;
bool init();
bool loadMedia();
void close();

int pocketNumber = 1;
int breakTime = 2000;
int startTime = 0;
int target[3] = {0};
int posInPocket = 0;
int mouseX, mouseY, mouseState;
int crackFlag;
int blockMouseX, blockMouseY;
int absMouseX;
int absMouseY;//mouseState = 0/1/2/4  NULL/left/middle/right
int isTakenUp = 0;
int IDWithMouse = 0, numWithMouse = 0;
int heartFrame = 0;
double angle = 0;
double angleForBlock = 0;

bool init()
{
	if (mainMap.checkIfExist())
	{
		mainMap.mapRead();
	}
	else
	{
		mainMap.generateMap();
		mainMap.mapWrite(mainMap.mapData);
		mainMap.mapRead();
	}
	//init the itemList
	initItemList();
	
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
	savingControler.fileRead(target);
	player.mCollider.x = target[0];
	player.mCollider.y = target[1];
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
		gWindow = SDL_CreateWindow("Test1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	
	if (very_behind_background_texture.loadFromFile("images/very_behind_bg.png"))
	{
		very_behind_background_clips[0].x = 0;
		very_behind_background_clips[0].y = 0;
		very_behind_background_clips[0].w = 1800;
		very_behind_background_clips[0].h = 1196;
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

	if (rubbish_texture.loadFromFile("images/rubbish.png"))
	{
		rubbish_clips[0].x = 0;
		rubbish_clips[0].y = 0;
		rubbish_clips[0].w = 100;
		rubbish_clips[0].h = 100;
	}
	
	if (pocketUI_texture.loadFromFile("images/pocket.png"))
	{
		for (int i = 0; i < 3; i++)
		{
			pocketUI_clips[i].x = 100 * i;
			pocketUI_clips[i].y = 0;
			pocketUI_clips[i].w = 100;
			pocketUI_clips[i].h = 100;
		}
	}

	if (tool_texture.loadFromFile("images/tools.png"))
	{
		for (int i = 0; i < 1; i++)
		{
			tool_clips[i].x = 100 * i;
			tool_clips[i].y = 0;
			tool_clips[i].w = 100;
			tool_clips[i].h = 100;
		}
	}

	if (weapon_texture.loadFromFile("images/weapons.png"))
	{
		for (int i = 0; i < 1; i++)
		{
			weapon_clips[i].x = 100 * i;
			weapon_clips[i].y = 0;
			weapon_clips[i].w = 100;
			weapon_clips[i].h = 100;
		}
	}

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

	else
	{
		printf("Failed to load media\n");
	}
	if (!player.loadTexture() || !mainMap.loadTexture() || !testEnemy.loadTexture())
	{
		success = false;
	}
	return success;
}

void close()
{
	int data[3] = {0};
	data[0] = player.mCollider.x;
	data[1] = player.mCollider.y;
	savingControler.fileWrite(data);

	mainMap.mapWrite(mainMap.mapData);
	mainPocket.pocketWrite(mainPocket.pocketData);
	//Free loaded images
	very_behind_background_texture.free();
	//Destroy window	
	//Free loaded images

	//Free global font

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

Uint32 callback(Uint32 interval, void* param)
{
	for (int i = 0; i < 200; i++)
	{
		player.pickUpItem(&droppedItemList[i]);
		droppedItemList[i].move();
	}
	player.move();
	testEnemy.move();
	player.getHit(testEnemy);
	int deltaX = cam.countCompensateX(SCREEN_WIDTH, player.posX);
	int deltaY = cam.countCompensateY(SCREEN_HEIGHT, player.posY);

	SDL_Rect* generalPocketClip = &pocketUI_clips[0];
	SDL_Rect* highLightPocketClip = &pocketUI_clips[1];
	SDL_Rect* backpackPocketClip = &pocketUI_clips[2];
	SDL_Color textColorWhite = { 255, 255, 255 };
	SDL_Color textColorBlack = { 0, 0, 0 };

	for (int pocketPos = 0; pocketPos < 40; pocketPos++)
	{
		char str1[23];
		if (mainPocket.pocketData[1][pocketPos] < 10)
		{
			_itoa_s(mainPocket.pocketData[1][pocketPos], str1, 10);
			char newStr1[23] = " ";
			strcat_s(newStr1, str1);

			gTextTexture1[pocketPos].loadFromRenderedText(newStr1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(newStr1, textColorWhite);
		}
		else
		{
			_itoa_s(mainPocket.pocketData[1][pocketPos], str1, 10);
			//Render text
			gTextTexture1[pocketPos].loadFromRenderedText(str1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(str1, textColorWhite);
		}
	}

	/*������һ����Ⱦ�����Ĳ���*/
	very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE,2);
	mainMap.render(deltaX, deltaY);

	SDL_Rect* crackClip1 = &crack_clips[0];
	SDL_Rect* crackClip2 = &crack_clips[1];
	SDL_Rect* crackClip3 = &crack_clips[2];

	if (crackFlag && mainMap.mapData[blockMouseY][blockMouseX])
	{
		switch (crackFlag)
		{
		case 1:crack_texture.render(blockMouseX*50 + deltaX, blockMouseY*50 + deltaY, crackClip1, 0, NULL, SDL_FLIP_NONE, 2); break;
		case 2:crack_texture.render(blockMouseX*50 + deltaX, blockMouseY*50 + deltaY, crackClip2, 0, NULL, SDL_FLIP_NONE, 2); break;
		case 3:crack_texture.render(blockMouseX*50 + deltaX, blockMouseY*50 + deltaY, crackClip3, 0, NULL, SDL_FLIP_NONE, 2); break;
		}
	}

	for (int i = 0; i < 200; i++)
		droppedItemList[i].render(deltaX, deltaY);

	for (int p = 0; p < 10; p++)
	{
		pocketUI_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p, SCREEN_HEIGHT - 60, generalPocketClip, 0, NULL, SDL_FLIP_NONE, 2);

		if (mainPocket.pocketData[1][p]&& mainPocket.pocketData[0][p]<=100)
		{
			SDL_Rect* currentPocketClip = &mainMap.newMap_clips[mainPocket.pocketData[0][p]];
			mainMap.newMap_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 12, SCREEN_HEIGHT - 60 + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
		}
		if (mainPocket.pocketData[1][p] && mainPocket.pocketData[0][p] <= 100)
		{
			gTextTexture1[p].render(SCREEN_WIDTH / 2 - 250 + 26 + 50 * p, SCREEN_HEIGHT - 60 + 18, 0, 0, NULL, SDL_FLIP_NONE, 1);
			gTextTexture2[p].render(SCREEN_WIDTH / 2 - 250 + 24 + 50 * p, SCREEN_HEIGHT - 60 + 16, 0, 0, NULL, SDL_FLIP_NONE, 1);
		}
		if (mainPocket.pocketData[1][p] && mainPocket.pocketData[0][p] >= 300 && mainPocket.pocketData[0][p] < 400)
		{
			SDL_Rect* currentPocketClip = &tool_clips[mainPocket.pocketData[0][p]-300];
			tool_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 9, SCREEN_HEIGHT - 60 + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
		}

		if (mainPocket.pocketData[1][p] && mainPocket.pocketData[0][p] >= 400 && mainPocket.pocketData[0][p] < 500)
		{
			SDL_Rect* currentPocketClip = &weapon_clips[mainPocket.pocketData[0][p] - 400];
			weapon_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 9, SCREEN_HEIGHT - 60 + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
		}

		for (int q = 0; q < 3; q++)
		{
			if (mainPocket.isOpened)
			{
				pocketUI_texture.render(20 + 50 * p, 20 + 50 * q, backpackPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
			}
		}
	}
	if (pocketNumber > 0 && pocketNumber < 11)
	{
		pocketUI_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * (pocketNumber - 1), SCREEN_HEIGHT - 60, highLightPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
	}

	if (mainPocket.isOpened)
	{
		pocketUI_texture.render(20 + 50 * 9, 20 + 50 * 3, backpackPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
		for (int p = 10; p < 40; p++)
		{
			if (mainPocket.pocketData[1][p] && mainPocket.pocketData[0][p] <= 100)
			{
				SDL_Rect* currentPocketClip = &mainMap.newMap_clips[mainPocket.pocketData[0][p]];
				mainMap.newMap_texture.render(20 + 50 * (p % 10) + 12, 20 + 50 * (p/10-1) + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
				gTextTexture1[p].render(20 + 26 + 50 * (p % 10), 20 + 18 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTexture2[p].render(20 + 24 + 50 * (p % 10), 20 + 16 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
			}
			if (mainPocket.pocketData[1][p] && mainPocket.pocketData[0][p] > 300 && mainPocket.pocketData[0][p] <= 400)
			{
				SDL_Rect* currentPocketClip = &tool_clips[mainPocket.pocketData[0][p]-300];
				tool_texture.render(20 + 50 * (p % 10) + 9, 20 + 50 * (p / 10 - 1) + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			if (mainPocket.pocketData[1][p] && mainPocket.pocketData[0][p] > 400 && mainPocket.pocketData[0][p] <= 500)
			{
				SDL_Rect* currentPocketClip = &weapon_clips[mainPocket.pocketData[0][p] - 400];
				weapon_texture.render(20 + 50 * (p % 10) + 9, 20 + 50 * (p / 10 - 1) + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
		}
		rubbish_texture.render(20 + 50 * 9+9, 20 + 50 * 3+9, rubbish_clips, 0, NULL, SDL_FLIP_NONE, 3);
		if (isTakenUp)
		{
			if (IDWithMouse <= 100)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &mainMap.newMap_clips[IDWithMouse];
				mainMap.newMap_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);

				char str1[23];

				SDL_Color textColorWhite = { 255, 255, 255 };
				SDL_Color textColorBlack = { 0, 0, 0 };

				if (numWithMouse < 10)
				{
					_itoa_s(numWithMouse, str1, 10);
					char newStr1[23] = " ";
					strcat_s(newStr1, str1);

					gTextTextureOne.loadFromRenderedText(newStr1, textColorBlack);
					gTextTextureTwo.loadFromRenderedText(newStr1, textColorWhite);
				}
				else
				{
					_itoa_s(numWithMouse, str1, 10);
					//Render text
					gTextTextureOne.loadFromRenderedText(str1, textColorBlack);
					gTextTextureTwo.loadFromRenderedText(str1, textColorWhite);
				}

				gTextTextureOne.render(mouseX + 2 + 7, mouseY + 2 + 7, 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTextureTwo.render(mouseX + 7, mouseY + 7, 0, 0, NULL, SDL_FLIP_NONE, 1);

			}
			else if (IDWithMouse > 300 && IDWithMouse <= 400)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &tool_clips[IDWithMouse-300];
				tool_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			else if (IDWithMouse > 400 && IDWithMouse <= 500)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &weapon_clips[IDWithMouse - 400];
				weapon_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
		}
	}
	double percentage = (double)player.healthPoint / (double)player.healthLimit;
	hp_texture.renderWithScale(SCREEN_WIDTH / 2 + 130 + 30, 25, &heart_clips[0], 0, NULL, SDL_FLIP_NONE,0.4,2,percentage);

	hp_texture.render(int(SCREEN_WIDTH / 2 + 130+30+(250*percentage)), 25, &heart_clips[1], 0, NULL, SDL_FLIP_NONE, 2);

	SDL_Rect* currentHeartClip = &heart_clips[heartFrame / 8];
	heart_texture.render(SCREEN_WIDTH/2+130, 25, currentHeartClip, 0, NULL, SDL_FLIP_NONE, 2);
	++heartFrame;
	if (heartFrame / 8 >= 8)
	{
		heartFrame = 0;
	}

	SDL_Point centralPoint ;
	if (mainPocket.pocketData[0][pocketNumber - 1] > 0 && mainPocket.pocketData[0][pocketNumber - 1] <= 100)
	{
		if (player.acceleration > 0)
		{
			centralPoint.x = 25;
			centralPoint.y = 75;
			mainMap.newMap_texture.render(SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], angleForBlock -20, &centralPoint, SDL_FLIP_NONE, 2);
		}
		else if (player.acceleration < 0)
		{
			centralPoint.x = 25;
			centralPoint.y = 75;
			mainMap.newMap_texture.render(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], -angleForBlock +20, &centralPoint, SDL_FLIP_HORIZONTAL, 2);
		}
		else
		{
			int mouseX, mouseY, mouseState;
			mouseState = SDL_GetMouseState(&mouseX, &mouseY);


			if (mouseX > SCREEN_WIDTH / 2)
			{
				centralPoint.x = 25;
				centralPoint.y = 75;
				mainMap.newMap_texture.render(SCREEN_WIDTH / 2 + 15, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], angleForBlock-20, &centralPoint, SDL_FLIP_NONE, 2);
			}
			else
			{
				centralPoint.x = 25;
				centralPoint.y = 75;
				mainMap.newMap_texture.render(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 2 - 30, &mainMap.newMap_clips[mainPocket.pocketData[0][pocketNumber - 1]], -angleForBlock +20, &centralPoint, SDL_FLIP_HORIZONTAL, 2);
			}
		}
	}

	if (mainPocket.pocketData[0][pocketNumber - 1] > 300 && mainPocket.pocketData[0][pocketNumber - 1] <= 400)
	{
		if (player.acceleration > 0)
		{
			centralPoint.x = 25;
			centralPoint.y = 75;
			tool_texture.render(SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - 60, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1]-300], angle, &centralPoint, SDL_FLIP_NONE, 1);
		}
		else if (player.acceleration < 0)
		{
			centralPoint.x = 75;
			centralPoint.y = 75;
			tool_texture.render(SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 - 60, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1] - 300], -angle, &centralPoint, SDL_FLIP_HORIZONTAL, 1);
		}
		else
		{
			int mouseX, mouseY, mouseState;
			mouseState = SDL_GetMouseState(&mouseX, &mouseY);


			if (mouseX > SCREEN_WIDTH / 2)
			{
				centralPoint.x = 25;
				centralPoint.y = 75;
				tool_texture.render(SCREEN_WIDTH / 2 + 15, SCREEN_HEIGHT / 2 - 60, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1] - 300], angle, &centralPoint, SDL_FLIP_NONE, 1);
			}
			else
			{
				centralPoint.x = 75;
				centralPoint.y = 75;
				tool_texture.render(SCREEN_WIDTH / 2 - 115, SCREEN_HEIGHT / 2 - 60, &tool_clips[mainPocket.pocketData[0][pocketNumber - 1] - 300], -angle, &centralPoint, SDL_FLIP_HORIZONTAL, 1);
			}
		}
	}
	else if (mainPocket.pocketData[0][pocketNumber - 1] > 400 && mainPocket.pocketData[0][pocketNumber - 1] <= 500)
	{
		if (player.acceleration > 0)
		{
			centralPoint.x = 0;
			centralPoint.y = 100;
			weapon_texture.render(SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - 80, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], angle, &centralPoint, SDL_FLIP_NONE, 1);
		}
		else if (player.acceleration < 0)
		{
			centralPoint.x = 100;
			centralPoint.y = 100;
			weapon_texture.render(SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 - 80, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], -angle, &centralPoint, SDL_FLIP_HORIZONTAL, 1);
		}
		else
		{
			int mouseX, mouseY, mouseState;
			mouseState = SDL_GetMouseState(&mouseX, &mouseY);


			if (mouseX > SCREEN_WIDTH / 2)
			{
				centralPoint.x = 0;
				centralPoint.y = 100;
				weapon_texture.render(SCREEN_WIDTH / 2 + 15, SCREEN_HEIGHT / 2 - 80, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], angle, &centralPoint, SDL_FLIP_NONE, 1);
			}
			else
			{
				centralPoint.x = 100;
				centralPoint.y = 100;
				weapon_texture.render(SCREEN_WIDTH / 2 - 115, SCREEN_HEIGHT / 2 - 90, &weapon_clips[mainPocket.pocketData[0][pocketNumber - 1] - 400], -angle, &centralPoint, SDL_FLIP_HORIZONTAL, 1);
			}
		}
	}
	
	player.moveAction(deltaX,deltaY);
	testEnemy.moveAction(deltaX, deltaY);

	// Render the dropped items
	

	//Render the collision box
	SDL_Rect tempRect[16];
	for (int i = 0; i < 16; i++) 
	{
		tempRect[i] = player.rectArray[i];
		tempRect[i].x += deltaX;
		tempRect[i].y += deltaY;
	}
	//SDL_RenderDrawRects(gRenderer, tempRect, 16);
	//������Ⱦ������Ⱦ��ǰ����
	SDL_RenderPresent(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_TimerID timerID1 = SDL_AddTimer(20, callback, (void*)"ad");
	return 0;
}

Uint32 mouseTimerCallback(Uint32 interval, void* param)
{
	static int prevMouseState;
	static int prevBlockMouseX, prevBlockMouseY;
	static int flag = 0;
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	absMouseX = mouseX - cam.countCompensateX(SCREEN_WIDTH, player.posX);
	absMouseY = mouseY - cam.countCompensateY(SCREEN_HEIGHT, player.posY);
	blockMouseX = absMouseX / 50;
	blockMouseY = absMouseY / 50;
	//If the same button is still being pressed
	if (mouseState != prevMouseState)
	{
		angle = 0;
	}
	if (mouseState == prevMouseState)
	{
		//If pressed the left buttom
		if (mouseState == 1)
		{
			if (angle < 60)
			{
				angle += 6;
			}
			else
			{
				angle = -60;
			}
			//If the mouse is on the same place
			if (blockMouseX == prevBlockMouseX && blockMouseY == prevBlockMouseY&&mainPocket.pocketData[0][pocketNumber-1]>300&& mainPocket.pocketData[0][pocketNumber - 1] <= 400)
			{
				//Break the block if time is enough
				if (flag == 40)
				{
					mainMap.breakBlock(blockMouseX, blockMouseY);
					player.updateCollisionBox();
					mainPocket.pocketUpdate();
					//update dropped item's collision box
					for (int i = 0; i < 200; i++)
						droppedItemList[i].updateCollisionBox();
					flag = 0;
					crackFlag = 0;
					SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
					return 0;
				}
				// If time is not enough
				crackFlag = flag / 10;
				flag++;
				
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
			//If the mouse moved to another block
			flag = 0;
			crackFlag = 0;
			prevBlockMouseX = blockMouseX;
			prevBlockMouseY = blockMouseY;
			SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
			return 0;
		}
		else if (mouseState == 4 && mainPocket.pocketData[0][pocketNumber - 1] <= 300)
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
				prevBlockMouseX = blockMouseX;
				prevBlockMouseY = blockMouseY;
				SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
		}
		else if (!mouseState)
		{
			angleForBlock = 0;
			crackFlag = 0;
			return 0;
		}
		
	}
	flag = 0;
	crackFlag = 0;
	prevMouseState = mouseState;
	SDL_TimerID mouseTimer = SDL_AddTimer(15, mouseTimerCallback, (void*)mouseState);
	return 0;

	//SDL_TimerID mouseTimer = SDL_AddTimer(10, mouseTimerCallback, (void*)mouseState);
}

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//����ý��
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			//��ʼ����Ⱦʱ��
			SDL_TimerID timerID1 = SDL_AddTimer(10, callback, (void*)"ad");
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
						close();
					}

					if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						//Adjust the velocity
						switch (e.key.keysym.sym)
						{
						case SDLK_1: pocketNumber = 1; break;
						case SDLK_2: pocketNumber = 2; break;
						case SDLK_3: pocketNumber = 3; break;
						case SDLK_4: pocketNumber = 4; break;
						case SDLK_5: pocketNumber = 5; break;
						case SDLK_6: pocketNumber = 6; break;
						case SDLK_7: pocketNumber = 7; break;
						case SDLK_8: pocketNumber = 8; break;
						case SDLK_9: pocketNumber = 9; break;
						case SDLK_0: pocketNumber = 10; break;
						case SDLK_ESCAPE:
							if (mainPocket.isOpened == 0)
							{
								mainPocket.isOpened = 1;
							}
							else if(mainPocket.isOpened == 1)
							{
								mainPocket.isOpened = 0;
							}
							break;
						case SDLK_x: 
							int damagePoint = 5;
							if (player.healthPoint > damagePoint)
							{
								player.healthPoint -= 5;
								printf("5 points DAMAGE!\n");
							}
							else
							{
								player.healthPoint =0;
								printf("PLAYER DIED!\n");
							}
						}
					}
					if (e.type == SDL_MOUSEWHEEL)
					{
						if (e.wheel.y == -1)
						{
							if (pocketNumber == 10)
							{
								pocketNumber = 1;
							}
							else
							{
								pocketNumber++;
							}
						}
						else if (e.wheel.y == 1)
						{
							if (pocketNumber == 1)
							{
								pocketNumber = 10;
							}
							else
							{
								pocketNumber--;
							}
						}
					}
					if (e.type == SDL_MOUSEBUTTONDOWN&&!mainPocket.isOpened)
					{
						//Get mouse position
						mouseTimerCallback(0, &mouseState);
					}
					
					if (e.type == SDL_MOUSEBUTTONDOWN && mainPocket.isOpened)
					{
						int mouseX, mouseY, mouseState;
						posInPocket = 0;
						
						mouseState = SDL_GetMouseState(&mouseX, &mouseY);
						if (mouseX > 20 && mouseX < 520&&mouseY>20&&mouseY<170)
						{
							posInPocket = 10 + ((mouseY - 20) / 50) * 10 + (mouseX - 20) / 50 + 1;
							if (mainPocket.pocketData[1][posInPocket - 1]&&!isTakenUp)
							{
								isTakenUp = 1;
								IDWithMouse = mainPocket.pocketData[0][posInPocket - 1];
								numWithMouse = mainPocket.pocketData[1][posInPocket - 1];

								mainPocket.pocketData[0][posInPocket - 1] = 0;
								mainPocket.pocketData[1][posInPocket - 1] = 0;

							}
							else if (isTakenUp && mainPocket.pocketData[1][posInPocket - 1])
							{
								if (mainPocket.pocketData[0][posInPocket - 1]==IDWithMouse&& mainPocket.pocketData[1][posInPocket - 1] + numWithMouse<=99)
								{
									mainPocket.pocketData[1][posInPocket - 1] += numWithMouse;
									isTakenUp = 0;
								}
								else
								{
									int temp1 = mainPocket.pocketData[0][posInPocket - 1];
									int temp2 = mainPocket.pocketData[1][posInPocket - 1];

									mainPocket.pocketData[0][posInPocket - 1] = IDWithMouse;
									mainPocket.pocketData[1][posInPocket - 1] = numWithMouse;

									IDWithMouse = temp1;
									numWithMouse = temp2;

									isTakenUp = 1;
								}
							}
							else if (isTakenUp && !mainPocket.pocketData[1][posInPocket - 1])
							{
								mainPocket.pocketData[0][posInPocket - 1] = IDWithMouse;
								mainPocket.pocketData[1][posInPocket - 1] = numWithMouse;
								isTakenUp = 0;
							}
						}
						if (mouseX > 470 && mouseX < 520 && mouseY>170 && mouseY < 230&&isTakenUp)
						{
							IDWithMouse = 0;
							numWithMouse = 0;
							isTakenUp = 0;
						}
						if (mouseX > SCREEN_WIDTH / 2 - 250 && mouseX < SCREEN_WIDTH / 2 + 250&&mouseY>SCREEN_HEIGHT-60&&mouseY<SCREEN_HEIGHT-10)
						{
							posInPocket = (mouseX) / 50 - 4 + 1;
							if (mainPocket.pocketData[1][posInPocket - 1]&&!isTakenUp)
							{
								isTakenUp = 1;
								IDWithMouse = mainPocket.pocketData[0][posInPocket - 1];
								numWithMouse = mainPocket.pocketData[1][posInPocket - 1];


								mainPocket.pocketData[0][posInPocket - 1] = 0;
								mainPocket.pocketData[1][posInPocket - 1] = 0;

							}
							else if (isTakenUp && mainPocket.pocketData[1][posInPocket - 1])
							{
								if (mainPocket.pocketData[0][posInPocket - 1] == IDWithMouse&& mainPocket.pocketData[1][posInPocket - 1] + numWithMouse<=99)
								{
									mainPocket.pocketData[1][posInPocket - 1] += numWithMouse;
									isTakenUp = 0;
								}
								else
								{
									int temp1 = mainPocket.pocketData[0][posInPocket - 1];
									int temp2 = mainPocket.pocketData[1][posInPocket - 1];

									mainPocket.pocketData[0][posInPocket - 1] = IDWithMouse;
									mainPocket.pocketData[1][posInPocket - 1] = numWithMouse;

									IDWithMouse = temp1;
									numWithMouse = temp2;

									isTakenUp = 1;
								}
							}
							else if (isTakenUp && !mainPocket.pocketData[1][posInPocket - 1])
							{
								mainPocket.pocketData[0][posInPocket - 1] = IDWithMouse;
								mainPocket.pocketData[1][posInPocket - 1] = numWithMouse;
								isTakenUp = 0;
							}
						}	
					}
					player.handleEvent(e);
					
				}
				//SDL_Delay(10);
			}
			return 0;
		}
	}
}
/*********************************************************/