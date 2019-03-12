#pragma once
#include <SDL.h>
class pocket
{
public:
	pocket();
	~pocket();
	int isOpened=0;
	int pocketData[2][40];
	void pocketGenerate();
	void pocketRead();
	void pocketUpdate();
	void mainPocketRender();
	void handlePocketEvents(SDL_Event e);
	void pocketWrite(int targrtPocket[2][40]);
	int checkIfExist();
};

