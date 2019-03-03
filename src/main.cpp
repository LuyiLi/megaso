//头文件
#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "dot.h"
#include "LTtexture.h"
#include "settings.h"

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
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
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

void slime_move()
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
			int frame = 0;
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
				int delta_x = 0, delta_y = 0;
				int velocity = 3;
				delta_x = 40 / velocity;
				delta_y = velocity;
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					SDL_Rect* currentClip = &slime_walk_clips[frame / 4];
					SDL_RenderClear(gRenderer);
					very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
					background_texture.render(0, 400, background_clips, 0, NULL, SDL_FLIP_NONE);
					slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
					//Update screen
					SDL_RenderPresent(gRenderer);
					//Go to next frame
					SDL_Delay(delta_x);
					++frame;
					//Cycle animation
					if (frame / 4 >= walking_frames)
					{
						frame = 0;
					}

				}
				else if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					SDL_RenderClear(gRenderer);
					SDL_Rect* currentClip = &slime_walk_clips[frame / 4];
					very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
					background_texture.render(0, 400, background_clips, 0, NULL, SDL_FLIP_NONE);
					slime_walking_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_HORIZONTAL);
					//Update screen
					SDL_RenderPresent(gRenderer);
					//Go to next frame
					SDL_Delay(delta_x);
					++frame;
					//Cycle animation
					if (frame / 4 >= walking_frames)
					{
						frame = 0;
					}
				}
				else if (currentKeyStates[SDL_SCANCODE_UP])
				{
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
					//Render current frame
					SDL_Rect* currentClip = &slime_stand_clips[frame / 6];
					very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
					background_texture.render(0, 400, background_clips, 0, NULL, SDL_FLIP_NONE);
					slime_standing_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
					//Update screen
					SDL_RenderPresent(gRenderer);
					//Go to next frame
					SDL_Delay(12);
					++frame;
					//Cycle animation
					if (frame / 6 >= standing_frames)
					{
						frame = 0;
					}
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
				}
				else if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
					//Render current frame
					SDL_Rect* currentClip = &slime_stand_clips[frame / 6];
					very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
					background_texture.render(0, 400, background_clips, 0, NULL, SDL_FLIP_NONE);
					slime_standing_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
					//Update screen
					SDL_RenderPresent(gRenderer);
					//Go to next frame
					SDL_Delay(12);
					++frame;
					//Cycle animation
					if (frame / 6 >= standing_frames)
					{
						frame = 0;
					}
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
				}
				else
				{
					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
					SDL_Rect* currentClip = &slime_stand_clips[frame / 6];
					very_behind_background_texture.render(0, 0, very_behind_background_clips, 0, NULL, SDL_FLIP_NONE);
					background_texture.render(0, 400, background_clips, 0, NULL, SDL_FLIP_NONE);
					slime_standing_texture.render((pos_x), (pos_y), currentClip, 0, NULL, SDL_FLIP_NONE);
					//Update screen
					SDL_RenderPresent(gRenderer);
					//Go to next frame
					SDL_Delay(12);
					++frame;
					//Cycle animation
					if (frame / 6 >= standing_frames)
					{
						frame = 0;
					}
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
				}
			}
		}
	}

	//Free resources and close SDL
	close();
}

int main(int argc, char* args[])
{
	//Main loop flag
	bool quit = false;

	//Event handler
	//SDL_Event e;

	//The dot that will be moving around on the screen
	
	slime_move();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file



