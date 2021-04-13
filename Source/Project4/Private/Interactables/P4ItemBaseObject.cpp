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

void UP4ItemBaseObject::InitializeFromDataAsset(UItemBaseDataAsset* Data)
{
	if (Data)
	{
		ItemType = Data->ItemInfo.ItemType;
		bIsEmpty = false;
		bIsStackable = Data->ItemInfo.bIsStackable;
		MaxStackCount = Data->ItemInfo.MaxStackCount;
		DroppedItemClass = Data->ItemInfo.ItemClass;
		DroppedItemStaticMesh = Data->ItemInfo.ItemStaticMesh;
		ItemIcon = Data->ItemInfo.ItemIcon;
		ItemName = Data->ItemInfo.ItemName;
		ItemWeight = Data->ItemInfo.ItemWeight;
		ItemID = Data->ItemInfo.ItemID;
	}
}
