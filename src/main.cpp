#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Player.h"
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
#include "Enemy.h"

Item itemList[100];

droppedItem droppedItemList[200];



//�������
Player player;

//�������
Camera cam;

//�浵����������
SavingControl savingControler;

//״̬Ϊδ�˳�
bool quit = false;

//SDL��ʼ����������
bool init();

//ý����غ�������
bool loadMedia();

//�����˳���������
void close();

//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture1[10];
LTexture gTextTexture2[10];

//��Ⱦ���趨
SDL_Renderer* gRenderer = NULL;

//�����趨
SDL_Window* gWindow = NULL;

/*����ͼ����Ƭ*/
SDL_Rect background_clips[1];
LTexture background_texture;

SDL_Rect crack_clips[3];
LTexture crack_texture;

/*����ͼ����Ƭ*/
SDL_Rect very_behind_background_clips[1];
LTexture very_behind_background_texture;


SDL_Rect pocketUI_clips[2];
LTexture pocketUI_texture;

/*��ײ�����*/
LTexture gPlayerTexture;

/*��������ͼ*/
Map mainMap;

pocket mainPocket;

int pocketNumber = 1;
int breakTime = 2000;
int startTime = 0;
int target[3] = {0};

int mouseX, mouseY, mouseState;
int crackFlag;
int blockMouseX, blockMouseY;
int absMouseX;
int absMouseY;//mouseState = 0/1/2/4  NULL/left/middle/right

bool init()
{
	//���ɳ�ʼ��ͼ
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
	if (pocketUI_texture.loadFromFile("images/pocket.png"))
	{
		pocketUI_clips[0].x = 0;
		pocketUI_clips[0].y = 0;
		pocketUI_clips[0].w = 100;
		pocketUI_clips[0].h = 100;

		pocketUI_clips[1].x = 100;
		pocketUI_clips[1].y = 0;
		pocketUI_clips[1].w = 100;
		pocketUI_clips[1].h = 100;
	}

	if (crack_texture.loadFromFile("images/crack.png"))
	{
		crack_clips[0].x = 0;
		crack_clips[0].y = 0;
		crack_clips[0].w = 100;
		crack_clips[0].h = 100;

		crack_clips[1].x = 100;
		crack_clips[1].y = 0;
		crack_clips[1].w = 100;
		crack_clips[1].h = 100;

		crack_clips[2].x = 200;
		crack_clips[2].y = 0;
		crack_clips[2].w = 100;
		crack_clips[2].h = 100;
	}

	else
	{
		printf("SDL,TQL,WSL");
	}
	if (!player.loadTexture() || !mainMap.loadTexture())
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
	
	int deltaX = cam.countCompensateX(SCREEN_WIDTH, player.posX);
	int deltaY = cam.countCompensateY(SCREEN_HEIGHT, player.posY);

	SDL_Rect* generalPocketClip = &pocketUI_clips[0];
	SDL_Rect* highLightPocketClip = &pocketUI_clips[1];

	for (int pocketPos = 0; pocketPos < 10; pocketPos++)
	{
		char str1[23];

		SDL_Color textColorWhite = { 255, 255, 255 };
		SDL_Color textColorBlack = { 0, 0, 0 };

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

	for (int w = 0; w < 10; w++)
	{
		pocketUI_texture.render(SCREEN_WIDTH / 2 - 250 + 50*w, SCREEN_HEIGHT - 60, generalPocketClip, 0, NULL, SDL_FLIP_NONE,2);
	}
	if (pocketNumber > 0 && pocketNumber < 11)
	{
		pocketUI_texture.render(SCREEN_WIDTH / 2 - 250 + 50*(pocketNumber - 1), SCREEN_HEIGHT - 60, highLightPocketClip, 0, NULL, SDL_FLIP_NONE,2);
	}
	
	for (int p = 0; p < 10; p++)
	{
		if (mainPocket.pocketData[1][p])
		{
			SDL_Rect* currentPocketClip = &mainMap.newMap_clips[mainPocket.pocketData[0][p]];
			mainMap.newMap_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 12, SCREEN_HEIGHT - 60 + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
		}
	}
	for (int m = 0; m < 10; m++)
	{
		if (mainPocket.pocketData[1][m])
		{
			gTextTexture1[m].render(SCREEN_WIDTH / 2 - 250 + 26 + 50 * m, SCREEN_HEIGHT - 60 + 18, 0, 0, NULL, SDL_FLIP_NONE, 1);
			gTextTexture2[m].render(SCREEN_WIDTH / 2 - 250 + 24 + 50 * m, SCREEN_HEIGHT - 60 + 16, 0, 0, NULL, SDL_FLIP_NONE, 1);
		}
	}
	

	player.moveAction(deltaX,deltaY);

	// Render the dropped items
	for (int i = 0; i < 200; i++)
		droppedItemList[i].render(deltaX, deltaY);

	//Render the collision box
	SDL_Rect tempRect[16];
	for (int i = 0; i < 16; i++) 
	{
		tempRect[i] = player.rectArray[i];
		tempRect[i].x += deltaX;
		tempRect[i].y += deltaY;
	}
	SDL_RenderDrawRects(gRenderer, tempRect, 16);
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
	if (mouseState == prevMouseState)
	{
		//If pressed the left buttom
		if (mouseState == 1)
		{
			//If the mouse is on the same place
			if (blockMouseX == prevBlockMouseX && blockMouseY == prevBlockMouseY)
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
		else if (mouseState == 4)
		//keep putting things on the floor

			if (mainPocket.pocketData[1][pocketNumber - 1])
			{
			//mainMap.putBlock(blockMouseX, blockMouseY, pocketNumber);
			mainMap.putBlock(blockMouseX, blockMouseY, mainPocket.pocketData[0][pocketNumber-1]);
			
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
		else if (!mouseState)
		{
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
	return 0;
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
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						//Get mouse position
						mouseTimerCallback(0, &mouseState);
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