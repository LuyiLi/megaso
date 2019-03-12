#pragma once
#include "item.h"
#include <string.h>

extern Item itemList[500];

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
	
	//ID 1-100 is blocks
	for (int i = 1; i < 101; i++)
	{
		itemList[i].ID = i;
		itemList[i].itemType = ITEM_BLOCK;
	}
	//ID = 1
	strcpy_s(itemList[1].itemName,"Dirt");
	strcpy_s(itemList[1].itemDescription, "A fundemental block for constructing.");
	itemList[1].maxStack = 99;
	itemList[1].blockHardness = 2;

	//ID=2
	strcpy_s(itemList[2].itemName, "log");
	strcpy_s(itemList[2].itemDescription, "A wood for constructing.");
	itemList[2].maxStack = 99;
	itemList[2].blockHardness = 2;

	//ID=3
	strcpy_s(itemList[3].itemName, "bronze wood");
	strcpy_s(itemList[3].itemDescription, "Harder than normal wood!");
	itemList[3].maxStack = 99;
	itemList[3].blockHardness = 3;

	//ID=4
	strcpy_s(itemList[4].itemName, "iron wood");
	strcpy_s(itemList[4].itemDescription, "Harder than bronze wood! ");
	itemList[4].maxStack = 99;
	itemList[4].blockHardness = 4;

	//ID=5
	strcpy_s(itemList[5].itemName, "cotten candy wood");
	strcpy_s(itemList[5].itemDescription, "The wood in the snowfield");
	itemList[5].maxStack = 99;
	itemList[5].blockHardness =2;

	//ID=6
	strcpy_s(itemList[6].itemName, "crystallization of tree");
	strcpy_s(itemList[6].itemDescription, "The rarest and hardest wood!");
	itemList[6].maxStack = 99;
	itemList[6].blockHardness = 6;

	//ID=7
	strcpy_s(itemList[7].itemName, "rock");
	strcpy_s(itemList[7].itemDescription, "\0");
	itemList[7].maxStack = 99;
	itemList[7].blockHardness = 3;

	//ID=8
	strcpy_s(itemList[8].itemName, "clay");
	strcpy_s(itemList[8].itemDescription, "For building a door.");
	itemList[8].maxStack = 99;
	itemList[8].blockHardness = 3;

	//ID=9
	strcpy_s(itemList[9].itemName, "Magic clay");
	strcpy_s(itemList[9].itemDescription, "With some other materials, there will be some magic power.");
	itemList[9].maxStack = 99;
	itemList[9].blockHardness = 4;

	

////ID 101-200 is the wall
//
////ID 201-300 is material
//
	//ID=201
	strcpy_s(itemList[201].itemName, "fire-stone");
	strcpy_s(itemList[201].itemDescription, "A stone for making fire and a certain arrow.");
	itemList[201].maxStack = 99;
	
	//ID=202
	strcpy_s(itemList[202].itemName, "cymophane");
	strcpy_s(itemList[202].itemDescription, "An element for a certain helmet.");
	itemList[202].maxStack = 99;

	//ID=203
	strcpy_s(itemList[203].itemName, "Obsidian");
	strcpy_s(itemList[203].itemDescription, "An element for certain sword and bow.");
	itemList[203].maxStack = 99;
	

	//ID=204
	strcpy_s(itemList[204].itemName, "Yellow gem");
	strcpy_s(itemList[204].itemDescription, "An element for certain sword and helmet.");
	itemList[204].maxStack = 99;

	//ID=205
	strcpy_s(itemList[205].itemName, "Diamond");
	strcpy_s(itemList[205].itemDescription, "An element for certain helmet.");
	itemList[205].maxStack = 99;

	//ID=206
	strcpy_s(itemList[206].itemName, "Bronze");
	strcpy_s(itemList[206].itemDescription, "An element for some attacking/defending weapons.");
	itemList[206].maxStack = 99;


	//ID=207
	strcpy_s(itemList[207].itemName, "Iron");
	strcpy_s(itemList[207].itemDescription, "An element for pickaxes and some attacking/defending weapons.");
	itemList[207].maxStack = 99;

	//ID=208
	strcpy_s(itemList[208].itemName, "Gold");
	strcpy_s(itemList[208].itemDescription, "An element for pickaxes and some attacking/defending weapons.");
	itemList[208].maxStack = 99;

	//ID=209
	strcpy_s(itemList[209].itemName, "Thread");
	strcpy_s(itemList[209].itemDescription, "An element for slingshot.");
	itemList[209].maxStack = 99;


	//ID=210
	strcpy_s(itemList[210].itemName, "Pangolin scales");
	strcpy_s(itemList[210].itemDescription, "An element for helmet.");
	itemList[210].maxStack = 1;

	//ID=211
	strcpy_s(itemList[211].itemName, "Bronze sword hilt");
	strcpy_s(itemList[211].itemDescription, "An element for Bronze sword.");
	itemList[211].maxStack = 99;

	//ID=212
	strcpy_s(itemList[212].itemName, "Feather");
	strcpy_s(itemList[212].itemDescription, "An element for a certain arrow.");
	itemList[212].maxStack = 99;

	//ID=213
	strcpy_s(itemList[213].itemName, "Iron sword hilt");
	strcpy_s(itemList[213].itemDescription, "An element for Iron sword.");
	itemList[213].maxStack = 1;

//ID 301-400 is tools

	//ID=301
	strcpy_s(itemList[301].itemName, "Wooden sword");
	strcpy_s(itemList[301].itemDescription, "\0");
	itemList[301].maxStack = 1;

	//ID=302
	strcpy_s(itemList[302].itemName, "Bronze sword");
	strcpy_s(itemList[302].itemDescription, "Harder than wooden sword.");
	itemList[302].maxStack = 1;

	//ID=303
	strcpy_s(itemList[303].itemName, "Iron sword");
	strcpy_s(itemList[303].itemDescription, "Harder than Iron sword.");
	itemList[303].maxStack = 1;

	//ID=304
	strcpy_s(itemList[304].itemName, "Gold sword");
	strcpy_s(itemList[304].itemDescription, "The hardest sword.");
	itemList[304].maxStack = 1;

	
//ID 401-500 is weapons
}