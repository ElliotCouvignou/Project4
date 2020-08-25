// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/ItemArmorDataAsset.h"

UItemArmorDataAsset::UItemArmorDataAsset()
{
	// Set base info thats consistent for all armor items
	ItemInfo.bIsStackable = false;
	ItemInfo.ItemType = EItemType::Armor;
}