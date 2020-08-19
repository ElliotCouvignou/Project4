// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemBaseActor.h"
#include "Characters/P4PlayerCharacterBase.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

AP4ItemBaseActor::AP4ItemBaseActor()
{
	
}


void AP4ItemBaseActor::GrantUniqueItemAbility()
{
}


void AP4ItemBaseActor::OnItemEquipped()
{
}

void AP4ItemBaseActor::OnItemUnequipped()
{
}

void AP4ItemBaseActor::OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result)
{
	if (SourceActor)
	{
		UP4InventoryBagComponent* IBC = SourceActor->GetInventoryBagComponent();
		if (IBC)
		{
			int PlacedIdx; 
			IBC->AddItemToInventory(InventoryItemStruct, PlacedIdx, Result);
		}
	}
}
