#pragma once
#include "Item.h"
#include <string>
class Block :
	public Item
{
public:
	Block(int id);
	~Block();
	std::string texturePath;
	
	
};

