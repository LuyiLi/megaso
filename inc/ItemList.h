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
	//ID 401-500 is weapon
	for (int i = 401; i < 500; i++)
	{
		itemList[i].ID = i;
		itemList[i].itemType = ITEM_WEAPON;
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
	strcpy_s(itemList[8].itemName, "ore1");
	strcpy_s(itemList[8].itemDescription, "\0");
	itemList[8].maxStack = 99;
	itemList[8].blockHardness = 3;

	//ID=9
	strcpy_s(itemList[9].itemName, "ore2");
	strcpy_s(itemList[9].itemDescription, "\0");
	itemList[9].maxStack = 99;
	itemList[9].blockHardness = 3;

	//ID=10
	strcpy_s(itemList[10].itemName, "ore3");
	strcpy_s(itemList[10].itemDescription, "\0");
	itemList[10].maxStack = 99;
	itemList[10].blockHardness = 3;

	//ID=11
	strcpy_s(itemList[11].itemName, "ore4");
	strcpy_s(itemList[11].itemDescription, "\0");
	itemList[11].maxStack = 99;
	itemList[11].blockHardness = 3;

	//ID=12
	strcpy_s(itemList[12].itemName, "ore5");
	strcpy_s(itemList[12].itemDescription, "\0");
	itemList[12].maxStack = 99;
	itemList[12].blockHardness = 3;

	//ID=13
	strcpy_s(itemList[13].itemName, "ore6");
	strcpy_s(itemList[13].itemDescription, "\0");
	itemList[13].maxStack = 99;
	itemList[13].blockHardness = 3;

	//ID=14
	strcpy_s(itemList[14].itemName, "clay");
	strcpy_s(itemList[14].itemDescription, "For building a door.");
	itemList[14].maxStack = 99;
	itemList[14].blockHardness = 3;

	//ID=15
	strcpy_s(itemList[15].itemName, "Magic clay");
	strcpy_s(itemList[15].itemDescription, "With some other materials, there will be some magic power.");
	itemList[15].maxStack = 99;
	itemList[15].blockHardness = 4;

	

  //ID 101-200 is the wall

  //ID 201-300 is material

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
	strcpy_s(itemList[301].itemName, "Wooden Pickaxe");
	strcpy_s(itemList[301].itemDescription, "\0");
	itemList[301].maxStack = 1;

	//ID=302
	strcpy_s(itemList[302].itemName, "Iron Pickaxe");
	strcpy_s(itemList[302].itemDescription, "Harder than Wooden Pickaxe.");
	itemList[302].maxStack = 1;

	//ID=303
	strcpy_s(itemList[303].itemName, "Gold Pickaxe");
	strcpy_s(itemList[303].itemDescription, "The hardest pickaxe.");
	itemList[303].maxStack = 1;

	//ID=304
	strcpy_s(itemList[304].itemName, "ax");
	strcpy_s(itemList[304].itemDescription, "Cut down trees.");
	itemList[304].maxStack = 1;

	//ID=305
	strcpy_s(itemList[305].itemName, "Torch");
	strcpy_s(itemList[305].itemDescription, "\0");
	itemList[305].maxStack = 99;

	//ID=306
	strcpy_s(itemList[306].itemName, "Door");
	strcpy_s(itemList[306].itemDescription, "\0");
	itemList[306].maxStack = 99;

	

	
//ID 401-500 is weapons
	//ID=401
	strcpy_s(itemList[401].itemName, "Wooden sword");
	strcpy_s(itemList[401].itemDescription, "\0");
	itemList[401].maxStack = 1;
	itemList[401].weaponDamage = 1;

	//ID=402
	strcpy_s(itemList[402].itemName, "Bronze sword");
	strcpy_s(itemList[402].itemDescription, "Harder than wooden sword.");
	itemList[402].maxStack = 1;
	itemList[402].weaponDamage = 2;


	//ID=403
	strcpy_s(itemList[403].itemName, "Iron sword");
	strcpy_s(itemList[403].itemDescription, "Harder than Iron sword.");
	itemList[403].maxStack = 1;
	itemList[403].weaponDamage = 6;


	//ID=404
	strcpy_s(itemList[404].itemName, "Gold sword");
	strcpy_s(itemList[404].itemDescription, "The hardest sword.");
	itemList[404].maxStack = 1;
	itemList[404].weaponDamage = 15;
		
	//ID=405
	strcpy_s(itemList[405].itemName, "Small slingshot");
	strcpy_s(itemList[405].itemDescription, "\0");
	itemList[405].maxStack = 1;
	itemList[405].weaponDamage = 3;

	//ID=406
	strcpy_s(itemList[406].itemName, "Small slingshot");
	strcpy_s(itemList[406].itemDescription, "\0");
	itemList[406].maxStack = 1;
	itemList[406].weaponDamage = 3;

	//ID=407
	strcpy_s(itemList[407].itemName, "Big slingshot");
	strcpy_s(itemList[407].itemDescription, "Better than the small slingshot.");
	itemList[407].maxStack = 1;
	itemList[407].weaponDamage = 5;

	//ID=408
	strcpy_s(itemList[408].itemName, "Wooden dart");
	strcpy_s(itemList[408].itemDescription, "\0");
	itemList[408].maxStack = 1;
	itemList[408].weaponDamage = 3;

	//ID=409
	strcpy_s(itemList[409].itemName, "Silver dart");
	strcpy_s(itemList[409].itemDescription, "Sharper than the wooden dart.");
	itemList[409].maxStack = 1;
	itemList[409].weaponDamage = 6;

	//ID=410
	strcpy_s(itemList[410].itemName, "Wooden bow");
	strcpy_s(itemList[410].itemDescription, "\0");
	itemList[410].maxStack = 1;
	itemList[410].weaponDamage = 5;

	//ID=411
	strcpy_s(itemList[411].itemName, "Bronzen bow");
	strcpy_s(itemList[411].itemDescription, "Shoot faster than Wooden bow.");
	itemList[411].maxStack = 1;
	itemList[411].weaponDamage = 8;

	//ID=412
	strcpy_s(itemList[412].itemName, "Iron bow");
	strcpy_s(itemList[412].itemDescription, "The most offensive bow.");
	itemList[412].maxStack = 1;
	itemList[412].weaponDamage = 12;

	//ID=413
	strcpy_s(itemList[413].itemName, "Arrow");
	strcpy_s(itemList[413].itemDescription, "\0");
	itemList[413].maxStack = 1;
	itemList[413].weaponDamage = 1;

	//ID=414
	strcpy_s(itemList[414].itemName, "Fire arrow");
	strcpy_s(itemList[414].itemDescription, "The most offensive bow.");
	itemList[414].maxStack = 1;
	itemList[414].weaponDamage = 12;

	//ID=415
	strcpy_s(itemList[415].itemName, "Iron arrow");
	strcpy_s(itemList[415].itemDescription, "The sharpest arrow.");
	itemList[415].maxStack = 1;
	itemList[415].weaponDamage = 12;

	//ID=416
	strcpy_s(itemList[416].itemName, "Bronze helmet");
	strcpy_s(itemList[416].itemDescription, "A normal defender.");
	itemList[416].maxStack = 1;
	itemList[416].weaponDamage = 2;

	//ID=417
	strcpy_s(itemList[417].itemName, "Iron helmet");
	strcpy_s(itemList[417].itemDescription, "A harder defender.");
	itemList[417].maxStack = 1;
	itemList[417].weaponDamage = 6;

	//ID=418
	strcpy_s(itemList[418].itemName, "Gold helmet");
	strcpy_s(itemList[418].itemDescription, "The hardest defender.");
	itemList[418].maxStack = 1;
	itemList[418].weaponDamage = 15;

	//ID=419
	strcpy_s(itemList[419].itemName, "Wooden shield");
	strcpy_s(itemList[419].itemDescription, "A basic defender.");
	itemList[419].maxStack = 1;
	itemList[419].weaponDamage = 1;

	//ID=420
	strcpy_s(itemList[420].itemName, "Bronze shield");
	strcpy_s(itemList[420].itemDescription, "An improved defender.");
	itemList[420].maxStack = 1;
	itemList[420].weaponDamage = 2;

	//ID=421
	strcpy_s(itemList[421].itemName, "Iron shield");
	strcpy_s(itemList[421].itemDescription, "Better than Bronze shield.");
	itemList[421].maxStack = 1;
	itemList[421].weaponDamage = 6;

	//ID=422
	strcpy_s(itemList[422].itemName, "Gold shield");
	strcpy_s(itemList[422].itemDescription, "The hardest shield.");
	itemList[422].maxStack = 1;
	itemList[422].weaponDamage = 15;
}