#pragma once
#include "item.h"
#include <string.h>

extern Item itemList[100];

/*
This itemList start from one.
Leave itemList[0] empty
*/

void initItemList()
{
	//ID = 1
	itemList[1].ID = 1;
	itemList[1].itemType = ITEM_BLOCK;
	strcpy_s(itemList[1].itemName,"Dirt");
	strcpy_s(itemList[1].itemDescription, "A fundemental block for constructing");
	itemList[1].maxStack = 99;
	itemList[1].blockHardness = 2;
}