// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemBaseObject.h"


UP4ItemBaseObject::UP4ItemBaseObject()
	: Super()
{
	ItemType = EItemType::None;
	bIsEmpty = true;
	bIsStackable = false;
	ItemID = -1;
}