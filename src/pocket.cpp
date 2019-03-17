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
extern SDL_Rect tool_clips[1];
extern LTexture tool_texture;
extern SDL_Rect weapon_clips[1];
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
		if (pocketData[1][p] && pocketData[0][p] >= 300 && pocketData[0][p] < 400)
		{
			SDL_Rect* currentPocketClip = &tool_clips[pocketData[0][p] - 300];
			tool_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 9, SCREEN_HEIGHT - 60 + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
		}

		if (pocketData[1][p] && pocketData[0][p] >= 400 && pocketData[0][p] < 500)
		{
			SDL_Rect* currentPocketClip = &weapon_clips[pocketData[0][p] - 400];
			weapon_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * p + 9, SCREEN_HEIGHT - 60 + 9, currentPocketClip, 0, NULL, SDL_FLIP_NONE, 3);
		}

		for (int q = 0; q < 3; q++)
		{
			if (isOpened)
			{
				pocketUI_texture.render(20 + 50 * p, 20 + 50 * q, backpackPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
			}
		}
	}
	for (int q = 0; q < 3; q++)
	{
		if (isOpened)
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
	}
	if (pocketNumber > 0 && pocketNumber < 11)
	{
		pocketUI_texture.render(SCREEN_WIDTH / 2 - 250 + 50 * (pocketNumber - 1), SCREEN_HEIGHT - 60, highLightPocketClip, 0, NULL, SDL_FLIP_NONE, 2);
	}

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
		}
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
		}
	}
}

void pocket::handlePocketEvents(SDL_Event e)
{
	pocketUpdate();
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
		case SDLK_x: 
			player.getKilled();
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
			if (pocketData[1][posInPocket - 1] && !isTakenUp)
			{
				isTakenUp = 1;
				IDWithMouse = pocketData[0][posInPocket - 1];
				numWithMouse = pocketData[1][posInPocket - 1];

				pocketData[0][posInPocket - 1] = 0;
				pocketData[1][posInPocket - 1] = 0;
			}
			else if (isTakenUp && pocketData[1][posInPocket - 1])
			{
				if (pocketData[0][posInPocket - 1] == IDWithMouse && pocketData[1][posInPocket - 1] + numWithMouse <= 99)
				{
					pocketData[1][posInPocket - 1] += numWithMouse;
					isTakenUp = 0;
				}
				else
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
			else if (isTakenUp && !pocketData[1][posInPocket - 1])
			{
				pocketData[0][posInPocket - 1] = IDWithMouse;
				pocketData[1][posInPocket - 1] = numWithMouse;
				isTakenUp = 0;
			}
		}
		if (mouseX > 470 && mouseX < 520 && mouseY>170 && mouseY < 230 && isTakenUp)
		{
			IDWithMouse = 0;
			numWithMouse = 0;
			isTakenUp = 0;
		}
		if (mouseX > SCREEN_WIDTH / 2 - 250 && mouseX < SCREEN_WIDTH / 2 + 250 && mouseY>SCREEN_HEIGHT - 60 && mouseY < SCREEN_HEIGHT - 10)
		{
			posInPocket = (mouseX) / 50 - 4 + 1;
			if (pocketData[1][posInPocket - 1] && !isTakenUp)
			{
				isTakenUp = 1;
				IDWithMouse = pocketData[0][posInPocket - 1];
				numWithMouse = pocketData[1][posInPocket - 1];


				pocketData[0][posInPocket - 1] = 0;
				pocketData[1][posInPocket - 1] = 0;

			}
			else if (isTakenUp && pocketData[1][posInPocket - 1])
			{
				if (pocketData[0][posInPocket - 1] == IDWithMouse && pocketData[1][posInPocket - 1] + numWithMouse <= 99)
				{
					pocketData[1][posInPocket - 1] += numWithMouse;
					isTakenUp = 0;
				}
				else
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
			else if (isTakenUp && !pocketData[1][posInPocket - 1])
			{
				pocketData[0][posInPocket - 1] = IDWithMouse;
				pocketData[1][posInPocket - 1] = numWithMouse;
				isTakenUp = 0;
			}
		}
		if (mouseX > 20 && mouseX < 170 && mouseY>220 && mouseY < 270)
		{
			materialPos = (mouseX - 20) / 50;
			printf("material %d\n", materialPos);
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
		if (mouseX > 20 && mouseX < 320 && mouseY>280 && mouseY < 380)
		{
			craftPos = 6 * ((mouseY - 280) / 50) + (mouseX - 20) / 50;
			printf("craft %d\n", craftPos);
		}

		if (mouseX > 700 && mouseX < 850 && mouseY>160 && mouseY < 210)
		{
			accessoriesPos = (mouseX - 700) / 50;
			printf("accessories %d\n", accessoriesPos);
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

void pocket::pocketUpdate()
{
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
