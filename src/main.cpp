//ͷ�ļ�
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

/*��ͼ���ʰ�*/
SDL_Rect mapClips[2];
LTexture mapTexture;

/*��������ͼ*/
Map mainMap;


int target[3] = {0};

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
	if (mapTexture.loadFromFile("images/mapTexture.png"))
	{
		mapClips[0].x = 0;
		mapClips[0].y = 0;
		mapClips[0].w = 200;
		mapClips[0].h = 200;

		mapClips[1].x = 200;
		mapClips[1].y = 0;
		mapClips[1].w = 200;
		mapClips[1].h = 200;
	}
	else
	{
		printf("SDL,TQL,WSL");
	}
	if (!player.initPlayerTexture())
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
	int deltaX = cam.countCompensateX(SCREEN_WIDTH, player.posX);
	int deltaY = cam.countCompensateY(SCREEN_HEIGHT, player.posY);
	player.move();
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	/*������һ����Ⱦ�����Ĳ���*/
	very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
	int absX = 0, absY = 0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			
			if (mainMap.mapData[i][j])
			{
				
				SDL_Rect* currentClip = &mapClips[mainMap.mapData[i][j]-1];
				mapTexture.render(absY + deltaX, absX + deltaY, currentClip, 0, NULL, SDL_FLIP_NONE);
				
			}
			absY += 100;
		}
		absY = 0;
		absX += 100;
	}
	player.moveAction(deltaX,deltaY);

	
	//������Ⱦ������Ⱦ��ǰ����
	SDL_RenderPresent(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_TimerID timerID1 = SDL_AddTimer(20, callback, (void*)"ad");
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
						int mouseX, mouseY;
						SDL_GetMouseState(&mouseX, &mouseY);
						int absMouseX = mouseX - cam.countCompensateX(SCREEN_WIDTH, player.posX);
						int absMouseY = mouseY - cam.countCompensateY(SCREEN_HEIGHT, player.posY);
						if (e.button.button == SDL_BUTTON_LEFT)
						{
							printf("%d %d DELETE\n", absMouseX / 100, absMouseY / 100);
						}
						if (e.button.button == SDL_BUTTON_RIGHT)
						{
							printf("%d %d CREATE\n", absMouseX / 100, absMouseY / 100);
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