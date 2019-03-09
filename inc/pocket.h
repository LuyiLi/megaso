#pragma once
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
	void pocketWrite(int targrtPocket[2][40]);
	int checkIfExist();
};

