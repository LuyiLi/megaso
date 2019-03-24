#pragma once
#include <SDL.h>
class pocket
{
public:
	pocket();
	~pocket();
	int isOpened=0;
	int pocketData[2][40] = {0};
	int materialData[2][3] = {0};
	int craftData[2][12] = {0};
	int accessories = 0;
	int posInPocket = 0;
	int materialPos = 0;
	int craftPos = 0;
	int accessoriesPos = 0;
	void pocketGenerate();
	void pocketRead();
	void pocketUpdate();
	void mainPocketRender();
	void handlePocketEvents(SDL_Event e);
	void composingTableUpdate();
	void pocketWrite(int targrtPocket[2][40]);
	int checkIfExist();
	bool loadTexture();
};

