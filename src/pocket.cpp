#include "pch.h"
#include "pocket.h"
#include <stdio.h>
#include "Map.h"
#include "settings.h"
#include "Entity.h"

pocket::pocket()
{

}
void pocket::pocketGenerate()
{
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			pocketData[j][i] = 0;
		}
	}
}

extern Map mainMap;
extern int pocketNumber;
extern int isTakenUp;
extern int IDWithMouse;
extern int numWithMouse;
extern SDL_Rect tool_clips[10];
extern LTexture tool_texture;
extern SDL_Rect weapon_clips[6];
extern LTexture weapon_texture;

extern int posInPocket;
extern Player player;

SDL_Rect pocketUI_clips[8];
LTexture pocketUI_texture;
SDL_Rect* generalPocketClip = &pocketUI_clips[0];
SDL_Rect* highLightPocketClip = &pocketUI_clips[1];
SDL_Rect* backpackPocketClip = &pocketUI_clips[2];
SDL_Rect* MaterialPocketClip = &pocketUI_clips[3];
SDL_Rect* CraftPocketClip = &pocketUI_clips[4];
SDL_Rect* CraftTableClip = &pocketUI_clips[5];
SDL_Rect* AccessoriesClip = &pocketUI_clips[6];
SDL_Rect* AccessoriesTitleClip = &pocketUI_clips[7];

SDL_Rect accessories_clips[6];
LTexture accessories_texture;

SDL_Rect material_clips[23];
LTexture material_texture;

SDL_Color textColorWhite = { 255, 255, 255 };
SDL_Color textColorBlack = { 0, 0, 0 };
LTexture gTextTexture1[80];
LTexture gTextTexture2[80];
LTexture gTextTextureOne;
LTexture gTextTextureTwo;
SDL_Rect rubbish_clips[1];
LTexture rubbish_texture;



void pocket::mainPocketRender()
{
	composingTableUpdate();
	if (pocketUI_texture.loadFromFile("images/pocket.png"))
	{
		for (int i = 0; i < 5; i++)
		{
			pocketUI_clips[i].x = 100 * i;
			pocketUI_clips[i].y = 0;
			pocketUI_clips[i].w = 100;
			pocketUI_clips[i].h = 100;
		}
		pocketUI_clips[5].x = 500;
		pocketUI_clips[5].y = 0;
		pocketUI_clips[5].w = 300;
		pocketUI_clips[5].h = 100;

		pocketUI_clips[6].x = 800;
		pocketUI_clips[6].y = 0;
		pocketUI_clips[6].w = 100;
		pocketUI_clips[6].h = 100;

		pocketUI_clips[7].x = 900;
		pocketUI_clips[7].y = 0;
		pocketUI_clips[7].w = 300;
		pocketUI_clips[7].h = 100;
	}

	if (accessories_texture.loadFromFile("images/accessories.png"))
	{
		for (int i = 0; i < 6; i++)
		{
			accessories_clips[i].x = 100*i;
			accessories_clips[i].y = 0;
			accessories_clips[i].w = 100;
			accessories_clips[i].h = 100;
		}
	}
	
	if (material_texture.loadFromFile("images/material.png"))
	{
		for (int i = 0; i < 23; i++)
		{
			material_clips[i].x = 100 * i;
			material_clips[i].y = 0;
			material_clips[i].w = 100;
			material_clips[i].h = 100;
		}
	}

	if (rubbish_texture.loadFromFile("images/rubbish.png"))
	{
		rubbish_clips[0].x = 0;
		rubbish_clips[0].y = 0;
		rubbish_clips[0].w = 100;
		rubbish_clips[0].h = 100;
	}

	for (int pocketPos = 0; pocketPos < 40; pocketPos++)
	{
		char str1[23];
		if (pocketData[1][pocketPos] < 10)
		{
			_itoa_s(pocketData[1][pocketPos], str1, 10);
			char newStr1[23] = " ";
			strcat_s(newStr1, str1);

			gTextTexture1[pocketPos].loadFromRenderedText(newStr1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(newStr1, textColorWhite);
		}
		else
		{
			_itoa_s(pocketData[1][pocketPos], str1, 10);
			//Render text
			gTextTexture1[pocketPos].loadFromRenderedText(str1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(str1, textColorWhite);
		}
	}
	for (int pocketPos = 40; pocketPos < 43; pocketPos++)
	{
		char str1[23];
		if (materialData[1][pocketPos-40] < 10)
		{
			_itoa_s(materialData[1][pocketPos - 40], str1, 10);
			char newStr1[23] = " ";
			strcat_s(newStr1, str1);

			gTextTexture1[pocketPos].loadFromRenderedText(newStr1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(newStr1, textColorWhite);
		}
		else
		{
			_itoa_s(materialData[1][pocketPos - 40], str1, 10);
			//Render text
			gTextTexture1[pocketPos].loadFromRenderedText(str1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(str1, textColorWhite);
		}
	}

	for (int pocketPos = 43; pocketPos < 55; pocketPos++)
	{
		char str1[23];
		if (craftData[1][pocketPos - 43] < 10)
		{
			_itoa_s(craftData[1][pocketPos - 40], str1, 10);
			char newStr1[23] = " ";
			strcat_s(newStr1, str1);

			gTextTexture1[pocketPos].loadFromRenderedText(newStr1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(newStr1, textColorWhite);
		}
		else
		{
			_itoa_s(craftData[1][pocketPos - 40], str1, 10);
			//Render text
			gTextTexture1[pocketPos].loadFromRenderedText(str1, textColorBlack);
			gTextTexture2[pocketPos].loadFromRenderedText(str1, textColorWhite);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//可取用背包内物品渲染
	/////////////////////////////////////////////////////////////////////////////////////////
	for (int p = 0; p < 10; p++)
	{
		pocketUI_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p, SCREEN_HEIGHT - 60, generalPocketClip, 0, NULL, SDL_FLIP_NONE, 2);

		if (pocketData[1][p] && pocketData[0][p] <= 100)
		{
			mainMap.newMap_texture.setColor(255, 255, 255);
			SDL_Rect* currentPocketClip = &mainMap.newMap_clips[pocketData[0][p]];
			mainMap.newMap_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 12, SCREEN_HEIGHT - 60 + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
		}
		if (pocketData[1][p] && pocketData[0][p] <= 100)
		{
			gTextTexture1[p].render(SCREEN_WIDTH / 2 - 250 + 26 + 50 * p, SCREEN_HEIGHT - 60 + 18, 0, 0, NULL, SDL_FLIP_NONE, 1);
			gTextTexture2[p].render(SCREEN_WIDTH / 2 - 250 + 24 + 50 * p, SCREEN_HEIGHT - 60 + 16, 0, 0, NULL, SDL_FLIP_NONE, 1);
		}
		if (pocketData[1][p] && pocketData[0][p] > 100 && pocketData[0][p] <= 200)
		{
			mainMap.wall_texture.setColor(255, 255, 255);
			SDL_Rect* currentPocketClip = &mainMap.wall_clips[pocketData[0][p] - 100];
			mainMap.wall_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 12, SCREEN_HEIGHT - 60 + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
		}
		if (pocketData[1][p] && pocketData[0][p] > 100 && pocketData[0][p] <= 200)
		{
			gTextTexture1[p].render(SCREEN_WIDTH / 2 - 250 + 26 + 50 * p, SCREEN_HEIGHT - 60 + 18, 0, 0, NULL, SDL_FLIP_NONE, 1);
			gTextTexture2[p].render(SCREEN_WIDTH / 2 - 250 + 24 + 50 * p, SCREEN_HEIGHT - 60 + 16, 0, 0, NULL, SDL_FLIP_NONE, 1);
		}

		if (pocketData[1][p] && pocketData[0][p] > 200 && pocketData[0][p] <= 300)
		{
			material_texture.setColor(255, 255, 255);
			SDL_Rect* currentPocketClip = &material_clips[pocketData[0][p] - 200];
			material_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 12, SCREEN_HEIGHT - 60 + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
		}
		if (pocketData[1][p] && pocketData[0][p] > 200 && pocketData[0][p] <= 300)
		{
			gTextTexture1[p].render(SCREEN_WIDTH / 2 - 250 + 26 + 50 * p, SCREEN_HEIGHT - 60 + 18, 0, 0, NULL, SDL_FLIP_NONE, 1);
			gTextTexture2[p].render(SCREEN_WIDTH / 2 - 250 + 24 + 50 * p, SCREEN_HEIGHT - 60 + 16, 0, 0, NULL, SDL_FLIP_NONE, 1);
		}

		if (pocketData[1][p] && pocketData[0][p] > 300 && pocketData[0][p] <= 400)
		{
			SDL_Rect* currentPocketClip = &tool_clips[pocketData[0][p] - 300];
			tool_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 9, SCREEN_HEIGHT - 60 + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
		}

		if (pocketData[1][p] && pocketData[0][p] > 400 && pocketData[0][p] <= 500)
		{
			SDL_Rect* currentPocketClip = &weapon_clips[pocketData[0][p] - 400];
			weapon_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 9, SCREEN_HEIGHT - 60 + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
		}
		if (pocketData[1][p] && pocketData[0][p] > 700 && pocketData[0][p] <= 800)
		{
			accessories_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 5, SCREEN_HEIGHT - 60 + 7, &accessories_clips[pocketData[0][p] - 701], 0, NULL, SDL_FLIP_NONE, 2.5);
		}

		for (int q = 0; q < 3; q++)
		{
			if (isOpened)
			{
				pocketUI_texture.render(20 + 50 * p, 20 + 50 * q, backpackPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//饰品栏渲染
	/////////////////////////////////////////////////////////////////////////////////////////
	if (isOpened)
	{
		for (int q = 0; q < 3; q++)
		{
			pocketUI_texture.render(20 + 50 * q, 220, MaterialPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
			for (int i = 0; i < 6; i++)
			{
				pocketUI_texture.render(20 + 50 * i, 280, CraftPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
				pocketUI_texture.render(20 + 50 * i, 330, CraftPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
			}
			pocketUI_texture.render(20, 180, CraftTableClip, 0, NULL, SDL_FLIP_NONE, 2);
			pocketUI_texture.render(700+50*q, 160, AccessoriesClip, 0, NULL, SDL_FLIP_NONE, 2);
			pocketUI_texture.render(700, 120, AccessoriesTitleClip, 0, NULL, SDL_FLIP_NONE, 2);
		}
		switch (accessories)
		{
		case 0:
			accessories_texture.render(704, 166, &accessories_clips[3], 0, NULL, SDL_FLIP_NONE, 2.5);
			break;
		case 1:
			accessories_texture.render(704, 166, &accessories_clips[0], 0, NULL, SDL_FLIP_NONE, 2.5);
			break;
		case 2:
			accessories_texture.render(704, 166, &accessories_clips[1], 0, NULL, SDL_FLIP_NONE, 2.5);
			break;
		case 3:
			accessories_texture.render(704, 166, &accessories_clips[2], 0, NULL, SDL_FLIP_NONE, 2.5);
			break;
		default:
			break;
		}
		accessories_texture.render(754, 166, &accessories_clips[4], 0, NULL, SDL_FLIP_NONE, 2.5);
		accessories_texture.render(805, 168, &accessories_clips[5], 0, NULL, SDL_FLIP_NONE, 2.5);
	}
	

	if (pocketNumber > 0 && pocketNumber < 11)
	{
		pocketUI_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * (pocketNumber - 1), SCREEN_HEIGHT - 60, highLightPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//主背包内物品渲染
	/////////////////////////////////////////////////////////////////////////////////////////
	if (isOpened)
	{
		pocketUI_texture.render(20 + 50 * 9, 20 + 50 * 3, backpackPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
		for (int p = 10; p < 40; p++)
		{
			if (pocketData[1][p] && pocketData[0][p] <= 100)
			{
				mainMap.newMap_texture.setColor(255, 255, 255);
				SDL_Rect* currentPocketClip = &mainMap.newMap_clips[pocketData[0][p]];
				mainMap.newMap_texture.render(20 + 50 * (p % 10) + 12, 20 + 50 * (p / 10 - 1) + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
				gTextTexture1[p].render(20 + 26 + 50 * (p % 10), 20 + 18 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTexture2[p].render(20 + 24 + 50 * (p % 10), 20 + 16 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
			}
			if (pocketData[1][p] && pocketData[0][p] > 100 && pocketData[0][p] <= 200)
			{
				mainMap.wall_texture.setColor(255, 255, 255);
				SDL_Rect* currentPocketClip = &mainMap.wall_clips[pocketData[0][p] - 100];
				mainMap.wall_texture.render(20 + 50 * (p % 10) + 12, 20 + 50 * (p / 10 - 1) + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
				gTextTexture1[p].render(20 + 26 + 50 * (p % 10), 20 + 18 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTexture2[p].render(20 + 24 + 50 * (p % 10), 20 + 16 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
			}

			if (pocketData[1][p] && pocketData[0][p] > 200 && pocketData[0][p] <= 300)
			{
				material_texture.setColor(255, 255, 255);
				SDL_Rect* currentPocketClip = &material_clips[pocketData[0][p] - 200];
				material_texture.render(20 + 50 * (p % 10) + 12, 20 + 50 * (p / 10 - 1) + 12, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);
				gTextTexture1[p].render(20 + 26 + 50 * (p % 10), 20 + 18 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTexture2[p].render(20 + 24 + 50 * (p % 10), 20 + 16 + 50 * (p / 10 - 1), 0, 0, NULL, SDL_FLIP_NONE, 1);
			}

			if (pocketData[1][p] && pocketData[0][p] > 300 && pocketData[0][p] <= 400)
			{
				SDL_Rect* currentPocketClip = &tool_clips[pocketData[0][p] - 300];
				tool_texture.render(20 + 50 * (p % 10) + 9, 20 + 50 * (p / 10 - 1) + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			if (pocketData[1][p] && pocketData[0][p] > 400 && pocketData[0][p] <= 500)
			{
				SDL_Rect* currentPocketClip = &weapon_clips[pocketData[0][p] - 400];
				weapon_texture.render(20 + 50 * (p % 10) + 9, 20 + 50 * (p / 10 - 1) + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			if (pocketData[1][p] && pocketData[0][p] > 700 && pocketData[0][p] <= 800)
			{
				accessories_texture.render(20 + 50 * (p % 10) + 5, 20 + 50 * (p / 10 - 1) + 7, &accessories_clips[pocketData[0][p] - 701], 0, NULL, SDL_FLIP_NONE, 2.5);
			}
		}


	/////////////////////////////////////////////////////////////////////////////////////////
	//材料栏内物品渲染
	/////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < 3; i++)
		{
			if (materialData[1][i] && materialData[0][i] <= 100)
			{
				mainMap.newMap_texture.setColor(255, 255, 255);
				SDL_Rect* currentmaterialClip = &mainMap.newMap_clips[materialData[0][i]];
				mainMap.newMap_texture.render(20 + 50 * i + 12, 235, currentmaterialClip, 0, NULL, SDL_FLIP_NONE, 4);
				gTextTexture1[i+40].render(20 + 26 + 50 * i, 235, 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTexture2[i+40].render(20 + 24 + 50 * i, 235, 0, 0, NULL, SDL_FLIP_NONE, 1);
			}
			if (materialData[1][i] && materialData[0][i] > 100 && materialData[0][i] <= 200)
			{
				mainMap.wall_texture.setColor(255, 255, 255);
				SDL_Rect* currentmaterialClip = &mainMap.wall_clips[materialData[0][i]-100];
				mainMap.wall_texture.render(20 + 50 * i + 12, 235, currentmaterialClip, 0, NULL, SDL_FLIP_NONE, 4);
				gTextTexture1[i + 40].render(20 + 26 + 50 * i, 235, 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTexture2[i + 40].render(20 + 24 + 50 * i, 235, 0, 0, NULL, SDL_FLIP_NONE, 1);
			}
			if (materialData[1][i] && materialData[0][i] > 200 && materialData[0][i] <= 300)
			{
				material_texture.setColor(255, 255, 255);
				SDL_Rect* currentmaterialClip = &material_clips[materialData[0][i] - 200];
				material_texture.render(20 + 50 * i + 12, 235, currentmaterialClip, 0, NULL, SDL_FLIP_NONE, 4);
				gTextTexture1[i + 40].render(20 + 26 + 50 * i, 235, 0, 0, NULL, SDL_FLIP_NONE, 1);
				gTextTexture2[i + 40].render(20 + 24 + 50 * i, 235, 0, 0, NULL, SDL_FLIP_NONE, 1);
			}

			if (materialData[1][i] && materialData[0][i] > 300 && materialData[0][i] <= 400)
			{
				SDL_Rect* currentmaterialClip = &tool_clips[materialData[0][i] - 300];
				tool_texture.render(20 + 50 * i + 9, 235, currentmaterialClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			if (materialData[1][i] && materialData[0][i] > 400 && materialData[0][i] <= 500)
			{
				SDL_Rect* currentmaterialClip = &weapon_clips[materialData[0][i] - 400];
				weapon_texture.render(20 + 50 * i + 9, 235, currentmaterialClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			
		}
		for (int i = 0; i < 6; i++)
		{
			if (craftData[0][i] && craftData[0][i] <= 100)
			{
				mainMap.newMap_texture.setColor(255, 255, 255);
				SDL_Rect* currentcraftClip = &mainMap.newMap_clips[craftData[0][i]];
				mainMap.newMap_texture.render(20 + 50 * i + 12, 292, currentcraftClip, 0, NULL, SDL_FLIP_NONE, 4);
			}
			if (craftData[0][i] && craftData[0][i] > 100&& craftData[0][i] && craftData[0][i] <= 200)
			{
				mainMap.wall_texture.setColor(255, 255, 255);
				SDL_Rect* currentcraftClip = &mainMap.wall_clips[craftData[0][i]-100];
				mainMap.wall_texture.render(20 + 50 * i + 12, 292, currentcraftClip, 0, NULL, SDL_FLIP_NONE, 4);
			}
			if (craftData[0][i] && craftData[0][i] > 200 && craftData[0][i] && craftData[0][i] <= 300)
			{
				material_texture.setColor(255, 255, 255);
				SDL_Rect* currentcraftClip = &material_clips[craftData[0][i] - 200];
				material_texture.render(20 + 50 * i + 12, 292, currentcraftClip, 0, NULL, SDL_FLIP_NONE, 4);
			}
			if (craftData[0][i] && craftData[0][i] > 300 && craftData[0][i] <= 400)
			{
				SDL_Rect* currentcraftClip = &tool_clips[craftData[0][i] - 300];
				tool_texture.render(20 + 50 * i + 9, 290, currentcraftClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			if (craftData[0][i] && craftData[0][i] > 400 && craftData[0][i] <= 500)
			{
				SDL_Rect* currentcraftClip = &weapon_clips[craftData[0][i] - 400];
				weapon_texture.render(20 + 50 * i + 9, 290, currentcraftClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			
		}
		rubbish_texture.render(20 + 50 * 9 + 9, 20 + 50 * 3 + 9, rubbish_clips, 0, NULL, SDL_FLIP_NONE, 3);
		if (isTakenUp)
		{
			if (IDWithMouse <= 100)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &mainMap.newMap_clips[IDWithMouse];
				mainMap.newMap_texture.setColor(255, 255, 255);
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
			else if (IDWithMouse > 100&& IDWithMouse <= 200)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &mainMap.newMap_clips[IDWithMouse-100];
				mainMap.wall_texture.setColor(255, 255, 255);
				mainMap.wall_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);

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
			else if (IDWithMouse > 200 && IDWithMouse <= 300)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &material_clips[IDWithMouse - 200];
				material_texture.setColor(255, 255, 255);
				material_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 4);

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
				SDL_Rect* currentPocketClip = &tool_clips[IDWithMouse - 300];
				tool_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			else if (IDWithMouse > 400 && IDWithMouse <= 500)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &weapon_clips[IDWithMouse - 400];
				weapon_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
			}
			else if (IDWithMouse > 700 && IDWithMouse <= 800)
			{
				int mouseX, mouseY, mouseState;
				mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				SDL_Rect* currentPocketClip = &accessories_clips[IDWithMouse - 701];
				accessories_texture.render(mouseX, mouseY, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 2.5);
			}
		}
	}
}

void pocket::composingTableUpdate()
{
	int sortedData[2][3];
	int temp1, temp2;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; ++j)
			sortedData[j][i] = materialData[j][i];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; ++j)
			if (sortedData[0][j] > sortedData[0][j + 1])
			{
				temp1 = sortedData[0][j + 1];
				temp2 = sortedData[1][j + 1];
				sortedData[0][j + 1] = sortedData[0][j];
				sortedData[1][j + 1] = sortedData[1][j];
				sortedData[0][j] = temp1;
				sortedData[1][j] = temp2;
			}
	}
	
	////////////////////////////////////////////////////////////
	//需要一个块合成的物品
	////////////////////////////////////////////////////////////
	if (sortedData[1][0]==0 && sortedData[1][1]==0 && sortedData[1][2])
	{
		//草方块合成
		if (sortedData[0][0] == 0 && sortedData[0][1] == 0 && sortedData[0][2] == 2)
		{
			craftData[0][0] = 1;
		}
		//一个木块合成一个木棍
		else if (sortedData[0][0] == 0 && sortedData[0][1] == 0 && sortedData[0][2] == 208)
		{
			craftData[0][0] = 207;
		}
	}
	////////////////////////////////////////////////////////////
	//需要两个块合成的物品
	////////////////////////////////////////////////////////////
	else if (sortedData[1][0]==0 && sortedData[1][1] && sortedData[1][2])
	{
		//两个木棍合成一个火把
		if (sortedData[0][0] == 0 && sortedData[0][1] == 207 && sortedData[0][2] == 207)
		{
			craftData[0][0] = 307;
		}
		//一个银树一个黑矿合成一个黑宝石
		if (sortedData[0][0] == 0 && sortedData[0][1] == 16 && sortedData[0][2] == 12)
		{
			craftData[0][0] = 201;
		}
		//一个金树一个红矿合成一个红宝石
		if (sortedData[0][0] == 0 && sortedData[0][1] == 18 && sortedData[0][2] == 8)
		{
			craftData[0][0] = 202;
		}
		//一个银树一个银矿合成一个银宝石
		if (sortedData[0][0] == 0 && sortedData[0][1] == 16 && sortedData[0][2] == 9)
		{
			craftData[0][0] = 203;
		}
		//一个蓝树一个绿矿合成一个绿宝石
		if (sortedData[0][0] == 0 && sortedData[0][1] == 20 && sortedData[0][2] == 11)
		{
			craftData[0][0] = 204;
		}
		//一个金树一个金矿合成一个金宝石
		if (sortedData[0][0] == 0 && sortedData[0][1] == 18 && sortedData[0][2] == 10)
		{
			craftData[0][0] = 205;
		}
	}
	////////////////////////////////////////////////////////////
	//需要三个块合成的物品
	////////////////////////////////////////////////////////////
	else if (sortedData[1][0] && sortedData[1][1] && sortedData[1][2])
	{
		//三个原木合成一个木块
		if (sortedData[0][0] == 104 && sortedData[0][1] == 104 && sortedData[0][2] == 104)
		{
			craftData[0][0] = 208;
		}
		//两个木块一个木棍合成木剑
		if (sortedData[0][0] == 207 && sortedData[0][1] == 208 && sortedData[0][2] == 208)
		{
			craftData[0][0] = 405;
		}
		//一个木块两个木棍合成木镐
		if (sortedData[0][0] == 207 && sortedData[0][1] == 207 && sortedData[0][2] == 208)
		{
			craftData[0][0] = 303;
		}
		//两个金树一个金矿合成一个金棍
		if (sortedData[0][0] == 18 && sortedData[0][1] == 18 && sortedData[0][2] == 10)
		{
			craftData[0][0] = 209;
		}
		//两个蓝树一个绿矿合成一个蓝棍
		if (sortedData[0][0] == 20 && sortedData[0][1] == 20 && sortedData[0][2] == 11)
		{
			craftData[0][0] = 210;
		}
		//一个蓝棍一个银矿一个银宝石合成一个银剑
		if (sortedData[0][0] == 210 && sortedData[0][1] == 9 && sortedData[0][2] == 203)
		{
			craftData[0][0] = 404;
		}
		//一个金棍一个红矿一个红宝石合成一个红剑
		if (sortedData[0][0] == 209 && sortedData[0][1] == 8 && sortedData[0][2] == 202)
		{
			craftData[0][0] = 403;
		}
		//一个金棍一个金矿一个金宝石合成一个金剑
		if (sortedData[0][0] == 209 && sortedData[0][1] == 10 && sortedData[0][2] == 205)
		{
			craftData[0][0] = 402;
		}
		//一个火龙果一个银剑一个金剑合成一个宝剑
		if (sortedData[0][0] == 212 && sortedData[0][1] == 404 && sortedData[0][2] == 402)
		{
			craftData[0][0] = 401;
		}
		//一个穿山甲鳞片一个银镐一个金镐合成一个神镐
		if (sortedData[0][0] == 206 && sortedData[0][1] == 304 && sortedData[0][2] == 302)
		{
			craftData[0][0] = 301;
		}
		//两个金树一个金宝石合成一个金镐
		if (sortedData[0][0] == 209 && sortedData[0][1] == 209 && sortedData[0][2] == 205)
		{
			craftData[0][0] = 302;
		}
		//两个蓝棍一个银宝石合成一个银镐
		if (sortedData[0][0] == 210 && sortedData[0][1] == 210 && sortedData[0][2] == 203)
		{
			craftData[0][0] = 304;
		}
	}
	////////////////////////////////////////////////////////////
	//不放物品时清空工作台
	////////////////////////////////////////////////////////////
	else
	{
		for (int i = 0; i < 6; i++)
		{
			craftData[0][i] = 0;
			craftData[1][i] = 0;
		}
	}
	
}

void pocket::handlePocketEvents(SDL_Event e)
{
	pocketUpdate();
	composingTableUpdate();
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
			if (isOpened == 0)
			{
				isOpened = 1;
			}
			else if (isOpened == 1)
			{
				isOpened = 0;
			}
			break;
		}
		
	}
	if (e.type == SDL_MOUSEBUTTONDOWN && isOpened)
	{
		
		int mouseX, mouseY, mouseState;
		posInPocket = 0;

		mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		if (mouseX > 20 && mouseX < 520 && mouseY>20 && mouseY < 170)
		{
			posInPocket = 10 + ((mouseY - 20) / 50) * 10 + (mouseX - 20) / 50 + 1;
			if (pocketData[1][posInPocket - 1] && !isTakenUp && mouseState==1)
			{
				isTakenUp = 1;
				IDWithMouse = pocketData[0][posInPocket - 1];
				numWithMouse = pocketData[1][posInPocket - 1];

				pocketData[0][posInPocket - 1] = 0;
				pocketData[1][posInPocket - 1] = 0;
			}
			else if (pocketData[1][posInPocket - 1] && !isTakenUp && mouseState == 4)
			{
				isTakenUp = 1;
				IDWithMouse = pocketData[0][posInPocket - 1];
				numWithMouse = 1;

				pocketData[1][posInPocket - 1] --;
			}
			else if (isTakenUp && pocketData[1][posInPocket - 1])
			{
				if (pocketData[0][posInPocket - 1] == IDWithMouse && pocketData[1][posInPocket - 1] + numWithMouse <= 99 && mouseState == 1)
				{
					pocketData[1][posInPocket - 1] += numWithMouse;
					isTakenUp = 0;
				}
				else if (pocketData[0][posInPocket - 1] == IDWithMouse && pocketData[1][posInPocket - 1] + numWithMouse <= 99 && mouseState == 4)
				{
					numWithMouse++;
					pocketData[1][posInPocket - 1]--;
					isTakenUp = 1;
				}
				else if(mouseState == 1)
				{
					int temp1 = pocketData[0][posInPocket - 1];
					int temp2 = pocketData[1][posInPocket - 1];

					pocketData[0][posInPocket - 1] = IDWithMouse;
					pocketData[1][posInPocket - 1] = numWithMouse;

					IDWithMouse = temp1;
					numWithMouse = temp2;

					isTakenUp = 1;
				}
			}
			else if (isTakenUp && !pocketData[1][posInPocket - 1] && mouseState == 1)
			{
				pocketData[0][posInPocket - 1] = IDWithMouse;
				pocketData[1][posInPocket - 1] = numWithMouse;
				numWithMouse = 0;
				IDWithMouse = 0;
				isTakenUp = 0;
			}
		}
		if (mouseX > 470 && mouseX < 520 && mouseY>170 && mouseY < 230 && isTakenUp && mouseState == 1)
		{
			IDWithMouse = 0;
			numWithMouse = 0;
			isTakenUp = 0;
		}
		if (mouseX > SCREEN_WIDTH / 2 - 250 && mouseX < SCREEN_WIDTH / 2 + 250 && mouseY>SCREEN_HEIGHT - 60 && mouseY < SCREEN_HEIGHT - 10)
		{
			posInPocket = (mouseX) / 50 - 4 + 1;
			if (pocketData[1][posInPocket - 1] && !isTakenUp && mouseState == 1)
			{
				isTakenUp = 1;
				IDWithMouse = pocketData[0][posInPocket - 1];
				numWithMouse = pocketData[1][posInPocket - 1];

				pocketData[0][posInPocket - 1] = 0;
				pocketData[1][posInPocket - 1] = 0;

			}

			else if (pocketData[1][posInPocket - 1] && !isTakenUp && mouseState == 4)
			{
				isTakenUp = 1;
				IDWithMouse = pocketData[0][posInPocket - 1];
				numWithMouse = 1;

				pocketData[1][posInPocket - 1] --;

			}

			else if (isTakenUp && pocketData[1][posInPocket - 1])
			{
				if (pocketData[0][posInPocket - 1] == IDWithMouse && pocketData[1][posInPocket - 1] + numWithMouse <= 99 && mouseState == 1)
				{
					pocketData[1][posInPocket - 1] += numWithMouse;
					isTakenUp = 0;
				}
				else if (pocketData[0][posInPocket - 1] == IDWithMouse && pocketData[1][posInPocket - 1] + numWithMouse <= 99 && mouseState == 4)
				{
					numWithMouse++;
					pocketData[1][posInPocket - 1]--;
					isTakenUp = 1;
				}
				else if(mouseState == 1)
				{
					int temp1 = pocketData[0][posInPocket - 1];
					int temp2 = pocketData[1][posInPocket - 1];

					pocketData[0][posInPocket - 1] = IDWithMouse;
					pocketData[1][posInPocket - 1] = numWithMouse;

					IDWithMouse = temp1;
					numWithMouse = temp2;

					isTakenUp = 1;
				}
			}
			else if (isTakenUp && !pocketData[1][posInPocket - 1] && mouseState == 1)
			{
				pocketData[0][posInPocket - 1] = IDWithMouse;
				pocketData[1][posInPocket - 1] = numWithMouse;
				numWithMouse = 0;
				IDWithMouse = 0;
				isTakenUp = 0;
			}
		}
		if (mouseX > 20 && mouseX < 170 && mouseY>220 && mouseY < 270 && mouseState == 1)
		{
			materialPos = (mouseX - 20) / 50;
			for (int i = 0; i < 3; i++)
			{
				if (!materialData[0][i]||!materialData[1][i])
				{
					materialData[0][i] = 0;
					materialData[1][i] = 0;
				}
			}
			
			if (materialData[1][materialPos] && !isTakenUp)
			{
				isTakenUp = 1;
				IDWithMouse = materialData[0][materialPos];
				numWithMouse = materialData[1][materialPos];

				materialData[0][materialPos] = 0;
				materialData[1][materialPos] = 0;
			}
			else if (isTakenUp && materialData[1][materialPos])
			{
				if (materialData[0][materialPos] == IDWithMouse && materialData[1][materialPos] + numWithMouse <= 99)
				{
					materialData[1][materialPos] += numWithMouse;
					isTakenUp = 0;
				}
				else
				{
					int temp1 = materialData[0][materialPos];
					int temp2 = materialData[1][materialPos];

					materialData[0][materialPos] = IDWithMouse;
					materialData[1][materialPos] = numWithMouse;

					IDWithMouse = temp1;
					numWithMouse = temp2;

					isTakenUp = 1;
				}
			}
			else if (isTakenUp && !materialData[1][materialPos])
			{
				materialData[0][materialPos] = IDWithMouse;
				materialData[1][materialPos] = numWithMouse;
				isTakenUp = 0;
			}
		}
		if (mouseX > 20 && mouseX < 320 && mouseY>280 && mouseY < 380 && mouseState == 1)
		{
			craftPos = 6 * ((mouseY - 280) / 50) + (mouseX - 20) / 50;
			if (!isTakenUp && craftData[0][craftPos])
			{
				IDWithMouse = craftData[0][craftPos];
				numWithMouse = 1;
				for (int p=0;p<3;p++)
				{
					if (materialData[0][p] && materialData[1][p])
					{
						materialData[1][p]--;
					}
				}
				isTakenUp = 1;
			}
			else if (isTakenUp && craftData[0][craftPos])
			{
				numWithMouse ++;
				for (int p = 0; p < 3; p++)
				{
					if (materialData[0][p] && materialData[1][p])
					{
						materialData[1][p]--;
					}
				}
				isTakenUp = 1;
			}
		}

		if (mouseX > 700 && mouseX < 850 && mouseY>160 && mouseY < 210 && mouseState == 1)
		{
			
			accessoriesPos = (mouseX - 700) / 50;
			if (accessoriesPos == 0 && accessories==0 && IDWithMouse > 700&&numWithMouse)
			{
				accessories = IDWithMouse - 700;
				IDWithMouse = 0;
				numWithMouse = 0;
				isTakenUp = 0;
				
			}
			else if (accessoriesPos == 0 && accessories && IDWithMouse == 0&&numWithMouse==0)
			{
				IDWithMouse = accessories + 700;
				numWithMouse = 1;
				accessories = 0;
				isTakenUp = 1;
			}
			else if (accessoriesPos == 0 && accessories && IDWithMouse&&numWithMouse)
			{
				int temp = accessories+700;
				accessories = IDWithMouse - 700;
				IDWithMouse = temp;
				numWithMouse = 1;
				isTakenUp = 1;
			}
		}
	}
}

int pocket::checkIfExist()
{
	FILE *fp;
	fopen_s(&fp, "pocket.txt", "r");
	if (fp == NULL)
	{
		return 0;
	}
	else
	{
		fclose(fp);
		return 1;
	}
}

extern Item itemList[500];

void pocket::pocketUpdate()
{
	player.currentItem = itemList[pocketData[0][pocketNumber - 1]];
	for (int i = 0; i < 40; i++)
	{
		if (pocketData[0][i] == 0 || pocketData[1][i] == 0)
		{
			pocketData[0][i] = 0;
			pocketData[1][i] = 0;
		}
	}
}

void pocket::pocketWrite(int targrtPocket[2][40])
{
	FILE *fp;
	fopen_s(&fp, "pocket.txt", "w");
	int m = 0, n = 0;
	int num = 0;
	for (m; m < 40; m++)
	{
		for (n; n < 2; n++)
		{
			num = targrtPocket[n][m];
			fprintf(fp, "%d,", num);
		}
		fprintf(fp, "R,");
		n = 0;
	}
	m = 0;
	fprintf(fp, "E");
	fclose(fp);
}

void pocket::pocketRead()
{
	FILE *fp;
	fopen_s(&fp, "pocket.txt", "r");
	char presentChar;
	int targetNum = 0;
	int num = 0;
	if (fp != NULL)
	{
		int m = 0, n = 0;
		int negativeFlag = 0;
		while ((presentChar = fgetc(fp)) != EOF)
		{
			if (presentChar == 'E')
			{
				fclose(fp);
				return;
			}
			else
			{
				if (presentChar != ',')
				{

					if (presentChar == '-')
					{
						negativeFlag = 1;
					}
					if ((int)presentChar >= 48 && (int)presentChar <= 57)
					{
						targetNum = targetNum * 10 + (int)presentChar - 48;
					}
					if (presentChar == 'R')
					{
						fgetc(fp);
						n = 0;
						m++;
					}
				}
				else if (presentChar == ',')
				{
					if (negativeFlag)
					{
						targetNum = targetNum * (-1);
					}
					negativeFlag = 0;
					pocketData[n][m] = targetNum;
					n++;
					targetNum = 0;
				}
			}
		}
		fclose(fp);
	}
}

pocket::~pocket()
{
}
