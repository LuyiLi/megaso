#pragma once
#include "LTexture.h"

enum ItemType
{
	ITEM_NULL = 0,
	ITEM_BLOCK = 1
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
	void drop();

	//The folling contents are for Blocks only
	char blockTexturePath[40];
	int blockHardness;
	int block;

};
