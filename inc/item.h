#pragma once
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
};

