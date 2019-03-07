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
#include "LTimer.h"
Item itemList[100];


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

//��Ⱦ���趨
SDL_Renderer* gRenderer = NULL;

//�����趨
SDL_Window* gWindow = NULL;

/*����ͼ����Ƭ*/
SDL_Rect background_clips[1];
LTexture background_texture;

/*����ͼ����Ƭ*/
SDL_Rect very_behind_background_clips[1];
LTexture very_behind_background_texture;

/*��ײ�����*/
LTexture gPlayerTexture;

/*��������ͼ*/
Map mainMap;

/*Creat a timer*/
LTimer timer;

int breakTime = 2000;
int startTime = 0;
int target[3] = {0};

int mouseX, mouseY, mouseState;
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
		mainMap.mapRead();
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
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	if (very_behind_background_texture.loadFromFile("images/very_behind_bg.png"))
	{
		very_behind_background_clips[0].x = 0;
		very_behind_background_clips[0].y = 0;
		very_behind_background_clips[0].w = 1800;
		very_behind_background_clips[0].h = 1196;
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
	int data[2333] = {0};
	data[0] = player.mCollider.x;
	data[1] = player.mCollider.y;
	savingControler.fileWrite(data);

	mainMap.mapWrite(mainMap.mapData);
	//Free loaded images
	//slime_standing_texture.free();
	//slime_walking_texture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

Uint32 callback(Uint32 interval, void* param)
{
	
	player.move();
	int deltaX = cam.countCompensateX(SCREEN_WIDTH, player.posX);
	int deltaY = cam.countCompensateY(SCREEN_HEIGHT, player.posY);

	/*������һ����Ⱦ�����Ĳ���*/
	very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
	mainMap.render(deltaX, deltaY);
	player.moveAction(deltaX,deltaY);

	
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
	static int blockMouseX, blockMouseY, prevBlockMouseX, prevBlockMouseY;
	static int flag = 0;
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	absMouseX = mouseX - cam.countCompensateX(SCREEN_WIDTH, player.posX);
	absMouseY = mouseY - cam.countCompensateY(SCREEN_HEIGHT, player.posY);
	blockMouseX = absMouseX / 100;
	blockMouseY = absMouseY / 100;
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
				if (flag == 30)
				{
					mainMap.breakBlock(blockMouseX, blockMouseY);
					player.updateCollisionBox();
					flag = 0;
					SDL_TimerID mouseTimer = SDL_AddTimer(20, mouseTimerCallback, (void*)mouseState);
					return 0;
				}
				// If time is not enough
				flag++;
				printf("%d\n", flag);
				SDL_TimerID mouseTimer = SDL_AddTimer(20, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
			//If the mouse moved to another block
			flag = 0;
			prevBlockMouseX = blockMouseX;
			prevBlockMouseY = blockMouseY;
			SDL_TimerID mouseTimer = SDL_AddTimer(20, mouseTimerCallback, (void*)mouseState);
			return 0;
		}
		else if (mouseState == 4)
		//keep putting things on the floor
			if (blockMouseX != prevBlockMouseX || blockMouseY != prevBlockMouseY)
			{
			mainMap.putBlock(blockMouseX, blockMouseY, 1);
			player.updateCollisionBox();
			prevBlockMouseX = blockMouseX;
			prevBlockMouseY = blockMouseY;
			SDL_TimerID mouseTimer = SDL_AddTimer(20, mouseTimerCallback, (void*)mouseState);
			return 0;
			}
			else
			{
				SDL_TimerID mouseTimer = SDL_AddTimer(20, mouseTimerCallback, (void*)mouseState);
				return 0;
			}
		else if (!mouseState)
		{
			return 0;
		}
		
	}
	flag = 0;
	prevMouseState = mouseState;
	SDL_TimerID mouseTimer = SDL_AddTimer(20, mouseTimerCallback, (void*)mouseState);
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
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						//Get mouse position
						mouseTimerCallback(0, &mouseState);
				
						if (e.button.button == SDL_BUTTON_RIGHT)
						{
							printf("%d %d CREATE\n", absMouseX / 100, absMouseY / 100);
							mainMap.putBlock(absMouseX / 100, absMouseY / 100, 1);
							player.updateCollisionBox();
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