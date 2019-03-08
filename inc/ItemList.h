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
	//ID = 0
	itemList[0].ID = 0;
	itemList[0].itemType = ITEM_NULL;
	itemList[0].itemName[0] = '\0';
	itemList[0].itemDescription[0] = '\0';
	itemList[0].maxStack = 0;
	//ID = 1
	itemList[1].ID = 1;
	itemList[1].itemType = ITEM_BLOCK;
	strcpy_s(itemList[1].itemName,"Dirt");
	strcpy_s(itemList[1].itemDescription, "A fundemental block for constructing");
	itemList[1].maxStack = 99;
	itemList[1].blockHardness = 2;
}