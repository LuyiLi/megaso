#pragma once
#include "LTexture.h"

enum ItemType
{
	ITEM_NULL = 0,
	ITEM_BLOCK = 1,
	ITEM_WALL = 2,
	ITEM_MATERIAL=3,
	ITEM_TOOL=4,
	ITEM_WEAPON=5
};


class Item
{
public:
	Item();
	~Item();
	int ID;
	ItemType itemType;
	char itemName[40];
	char itemDescription[2333];
	int maxStack;
	SDL_Rect itemClips[1];
	LTexture itemTexture;

	//The folling contents are for Blocks only
	char blockTexturePath[40];
	int blockHardness;
	

	//The following contents are for materials only
	
	//The following contents are for weapons only
	int weaponDamage;
};
