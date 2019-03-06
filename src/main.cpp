//头文件
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

//玩家设置
Player player;

//相机设置
Camera cam;

//碰撞墙面设置
SDL_Rect wall;

//存档控制器设置
SavingControl savingControler;

//状态为未退出
bool quit = false;

//SDL初始化函数声明
bool init();

//媒体加载函数声明
bool loadMedia();

//程序退出函数声明
void close();

//渲染器设定
SDL_Renderer* gRenderer = NULL;

//窗口设定
SDL_Window* gWindow = NULL;

/*地面图像切片*/
SDL_Rect background_clips[1];
LTexture background_texture;

/*背景图像切片*/
SDL_Rect very_behind_background_clips[1];
LTexture very_behind_background_texture;

/*碰撞点材质*/
LTexture gPlayerTexture;

/*地图材质包*/
SDL_Rect mapClips[2];
LTexture mapTexture;

/*创建主地图*/
Map mainMap;


int target[2333] = {0};

bool init()
{
	//生成初始地图
	//Initialization flag
	bool success = true;
	//savingControler.fileRead(target);
	player.mCollider.x = 0;
	player.mCollider.y = 0;
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
	player.move(wall);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	/*以下是一次渲染包含的材质*/
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
	
	
	//background_texture.render(deltaX, 400+deltaY, background_clips, 0, NULL, SDL_FLIP_NONE);

	player.moveAction(deltaX,deltaY);

	
	//更新渲染器，渲染当前材质
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
		//载入媒体
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			wall.x = 0;
			wall.y = 5000;
			wall.w = 10000;
			wall.h = 100;
			//初始化渲染时钟
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
					player.handleEvent(e);
				}
				//SDL_Delay(10);
			}
			return 0;
		}
	}
}
/*********************************************************/