//头文件
#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "dot.h"
#include "LTtexture.h"
#include "settings.h"

//碰撞点设置
Dot dot;
//碰撞墙面设置
SDL_Rect wall;
bool quit = false;
//SDL初始化函数声明
bool init();
bool loadMedia();
void close();
//渲染器设定
SDL_Renderer* gRenderer = NULL;
//窗口设定
SDL_Window* gWindow = NULL;
/*站立状态图像切片*/
const int standing_frames = 6;
SDL_Rect slime_stand_clips[standing_frames];
LTexture slime_standing_texture;
/*行走状态图像切片*/
const int walking_frames = 4;
SDL_Rect slime_walk_clips[walking_frames];
LTexture slime_walking_texture;
/*地面图像切片*/
SDL_Rect background_clips[1];
LTexture background_texture;
/*背景图像切片*/
SDL_Rect very_behind_background_clips[1];
LTexture very_behind_background_texture;
/*碰撞点材质*/
LTexture gDotTexture;

bool init()
{
	//Initialization flag
	bool success = true;

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
	//Load sprite sheet texture
	if (background_texture.loadFromFile("images/test_bg.png"))
	{
		background_clips[0].x = 0;
		background_clips[0].y = 0;
		background_clips[0].w = 2048;
		background_clips[0].h = 208;
	}

	if (!slime_walking_texture.loadFromFile("images/slime_walk.png"))
	{
		printf("Failed to load walking animation texture!\n");
		success = false;
	}
	else
	{
		//Set sprite clips
		slime_walk_clips[0].x = 0;
		slime_walk_clips[0].y = 0;
		slime_walk_clips[0].w = 394;
		slime_walk_clips[0].h = 394;

		slime_walk_clips[1].x = 394;
		slime_walk_clips[1].y = 0;
		slime_walk_clips[1].w = 394;
		slime_walk_clips[1].h = 394;

		slime_walk_clips[2].x = 788;
		slime_walk_clips[2].y = 0;
		slime_walk_clips[2].w = 394;
		slime_walk_clips[2].h = 394;

		slime_walk_clips[3].x = 1182;
		slime_walk_clips[3].y = 0;
		slime_walk_clips[3].w = 394;
		slime_walk_clips[3].h = 394;
	}
	//Load sprite sheet texture
	if (!slime_standing_texture.loadFromFile("images/slime_stand.png"))
	{
		printf("Failed to load walking animation texture!\n");
		success = false;
	}
	else
	{
		//Set sprite clips
		slime_stand_clips[0].x = 0;
		slime_stand_clips[0].y = 0;
		slime_stand_clips[0].w = 394;
		slime_stand_clips[0].h = 394;

		slime_stand_clips[1].x = 394;
		slime_stand_clips[1].y = 0;
		slime_stand_clips[1].w = 394;
		slime_stand_clips[1].h = 394;

		slime_stand_clips[2].x = 788;
		slime_stand_clips[2].y = 0;
		slime_stand_clips[2].w = 394;
		slime_stand_clips[2].h = 394;

		slime_stand_clips[3].x = 1182;
		slime_stand_clips[3].y = 0;
		slime_stand_clips[3].w = 394;
		slime_stand_clips[3].h = 394;

		slime_stand_clips[4].x = 1576;
		slime_stand_clips[4].y = 0;
		slime_stand_clips[4].w = 394;
		slime_stand_clips[4].h = 394;

		slime_stand_clips[5].x = 1970;
		slime_stand_clips[5].y = 0;
		slime_stand_clips[5].w = 394;
		slime_stand_clips[5].h = 394;
	}
	return success;
}

void close()
{
	//Free loaded images
	slime_standing_texture.free();
	slime_walking_texture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void update()
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
			Dot dot;
			//碰撞墙面设置
			SDL_Rect wall;
			wall.x = 0;
			wall.y = 290;
			wall.w = 1000;
			wall.h = 400;
			bool quit = false;
			SDL_Event e;
			int frame_walk = 0;
			int frame_stand = 0;
			int delta_x = 0, delta_y = 0;
			int velocity = 3;
			delta_x = 40 / velocity;
			delta_y = velocity;
			//主循环入口
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					dot.handleEvent(e);
				}
				dot.move(wall);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				/*以下是一次渲染包含的材质*/

				very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
				background_texture.render(0, 400, background_clips, 0, NULL, SDL_FLIP_NONE);

				if (dot.mVelX > 0)
				{
					SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
					slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
					++frame_walk;
					if (frame_walk / 4 >= walking_frames)
					{
						frame_walk = 0;
					}

				}
				else if (dot.mVelX < 0)
				{
					SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
					slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL);
					++frame_walk;
					if (frame_walk / 4 >= walking_frames)
					{
						frame_walk = 0;
					}
				}
				else
				{
					SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 6];
					slime_standing_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
					++frame_stand;
					if (frame_stand / 6 >= standing_frames)
					{
						frame_stand = 0;
					}
				}
				//更新渲染器，渲染当前材质
				SDL_RenderPresent(gRenderer);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				SDL_Delay(12);
			}
		}
	}
	//释放资源并结束SDL
	close();
}

Uint32 callback(Uint32 interval, void* param)
{
	static int frame_walk = 0;
	static int frame_stand = 0;
	dot.move(wall);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	/*以下是一次渲染包含的材质*/

	very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
	background_texture.render(0, 400, background_clips, 0, NULL, SDL_FLIP_NONE);

	if (dot.mVelX > 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
		++frame_walk;
		if (frame_walk / 4 >= walking_frames)
		{
			frame_walk = 0;
		}

	}
	else if (dot.mVelX < 0)
	{
		SDL_Rect* currentClip = &slime_walk_clips[frame_walk / 4];
		slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL);
		++frame_walk;
		if (frame_walk / 4 >= walking_frames)
		{
			frame_walk = 0;
		}
	}
	else
	{
		SDL_Rect* currentClip = &slime_stand_clips[frame_stand / 6];
		slime_standing_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
		++frame_stand;
		if (frame_stand / 6 >= standing_frames)
		{
			frame_stand = 0;
		}
	}
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
			int velocity = 3;
			SDL_Event e;
			wall.x = 0;
			wall.y = 290;
			wall.w = 1000;
			wall.h = 400;
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
					}
					dot.handleEvent(e);
				}
			}
			return 0;
		}
	}
}
/*********************************************************/