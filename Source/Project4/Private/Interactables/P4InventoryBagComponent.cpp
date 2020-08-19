// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4InventoryBagComponent.h"
#include "Containers/Array.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

// Sets default values for this component's properties
UP4InventoryBagComponent::UP4InventoryBagComponent(const class FObjectInitializer& ObjectInitializer)
{
	// You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UP4InventoryBagComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeEmptyInventory();
	
}

void UP4InventoryBagComponent::FindFirstEmptySpot(bool& WasSuccessful, int& Index)
{
	WasSuccessful = false;

	int idx = 0;
	for (FInventoryItemStruct& Item : InventoryArray)
	{
		if (Item.bIsEmpty)
		{
			WasSuccessful = true;
			Index = idx;
			return;
		}
		idx++;
	}
}

void UP4InventoryBagComponent::InitializeEmptyInventory()
{
	print(FString("INit"));
	InventoryArray.SetNum(NumInventorySlots, false);
	
	for (int i = 0; i < NumInventorySlots; i++)
	{
		InventoryArray[i] = FInventoryItemStruct();
	}
}

void UP4InventoryBagComponent::FindItemsFromItemData(const FInventoryItemStruct& SearchItem, bool& WasFound, TArray<int>& FoundIndexes)
{
	WasFound = false;

	int idx = 0;
	for (FInventoryItemStruct& Item : InventoryArray)
	{
		if (Item.ItemBaseDataAsset->ItemInfo.ItemID == SearchItem.ItemBaseDataAsset->ItemInfo.ItemID)
		{
			WasFound = true;
			FoundIndexes.Push(idx);
		}
		idx++;
	}
	
}

void UP4InventoryBagComponent::AddItemToInventory(UPARAM(ref)FInventoryItemStruct& NewItem, int& ReturnArrayindex, bool& WasSucessful)
{
	WasSucessful = false;

	if (NewItem.ItemBaseDataAsset && NewItem.ItemBaseDataAsset->ItemInfo.bIsStackable)
	{
		// Stackable, find locations that exists in inventory and stack (if max reached find next stack, if no stacks then do same approach as nonstackable (first available spot ))
		TArray<int> FoundIdxes;
		FindItemsFromItemData(NewItem, WasSucessful, FoundIdxes);

		if (WasSucessful)
		{			
			int StacksToFill = NewItem.StackCount;
			for (int idx : FoundIdxes)
			{
				//Stack Items
				InventoryArray[idx].StackCount += StacksToFill;
				OnInventorySlotUpdated.Broadcast(NewItem, idx);
				print(FString("Added + broadcast NOW (Stackable) "));

				// Check overflow
				if (InventoryArray[idx].StackCount > InventoryArray[idx].ItemBaseDataAsset->ItemInfo.MaxStackCount)
				{
					StacksToFill = InventoryArray[idx].StackCount - InventoryArray[idx].ItemBaseDataAsset->ItemInfo.MaxStackCount;
					InventoryArray[idx].StackCount = InventoryArray[idx].ItemBaseDataAsset->ItemInfo.MaxStackCount;
				}
				if (StacksToFill == 0)
					break;
			}
		}
	}
	else
	{
		// not stackable, place in first available slot, else return unsuccessful
		FindFirstEmptySpot(WasSucessful, ReturnArrayindex);
		if (WasSucessful)
		{
			InventoryArray.Insert(NewItem, ReturnArrayindex);
			OnInventorySlotUpdated.Broadcast(NewItem, ReturnArrayindex);
		}
	}
	
}

//void UP4InventoryBagComponent::AddItemToInventorySpot(const UItemBaseDataAsset* NewItem, const int ArrayIndex, bool& WasSucessful)
//{
//	if (NewItem)
//	{
//		// TODO: finish when adding is figured out 
//		//int idx = InventoryArray.Insert(NewItem, ArrayIndex);
//		//if (idx != -1)
//		//{
//		//	WasSucessful = true;
//		//	return;
//		//}
//		//else
//		//{
//		//	WasSucessful = false;
//		//	return;
//		//}
//	}
//}
//
//void UP4InventoryBagComponent::RemoveItemFromInventory(const UItemBaseDataAsset* RemovedItem, int ReturnArrayindex, bool& WasSucessful)
//{
//}
//
//void UP4InventoryBagComponent::RemoveItemFromInventorySpot(UItemBaseDataAsset* RemovedItem, const int ArrayIndex, bool& WasSucessful)
//{
//	if (RemovedItem)
//	{
//		InventoryArray.RemoveAt(ArrayIndex);
//		WasSucessful = true;
//
//
//		ItemRemovedDelegate.Broadcast(RemovedItem, ReturnArrayIndex);
//		
//		return;
//	}
//}




