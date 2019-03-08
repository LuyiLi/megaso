#pragma once
class pocket
{
public:
	pocket();
	~pocket();
	static const int pocketLength = 10;
	int pocketData[2][pocketLength];
	void pocketGenerate();
	void pocketRead();
	void pocketWrite(int targrtPocket[2][10]);
	int checkIfExist();
};

