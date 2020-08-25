// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4InventoryBagComponent.h"
#include "Engine/World.h"
#include "Interactables/P4ItemBaseActor.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

// Sets default values for this component's properties
UP4InventoryBagComponent::UP4InventoryBagComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
	InitializeEmptyEquippment();
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
	InventoryArray.SetNum(NumInventorySlots, false);
	
	for (int i = 0; i < NumInventorySlots; i++)
	{
		InventoryArray[i] = FInventoryItemStruct();
	}
}

void UP4InventoryBagComponent::InitializeEmptyEquippment()
{
	EquippmentSlots = FEquippedItemsStruct();

	// TODO: push empty weps here
	//EquippmentArray.Push(FEquippmentSlotStruct(EArmorType::Helmet));
	//EquippmentArray.Push(FEquippmentSlotStruct(EArmorType::Helmet));

}

bool UP4InventoryBagComponent::GetEquipSlotInfo(EArmorType ArmorType, bool IsRightFinger, FEquippmentSlotStruct& ReturnSlot)
{
	// These hardcoded values should reflect IntiializeEmptyEquippment order
	if (ArmorType == EArmorType::Helmet) {
		ReturnSlot = EquippmentSlots.HelmetSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Necklace) {
		ReturnSlot = EquippmentSlots.NecklaceSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Shoulder) {
		ReturnSlot = EquippmentSlots.ShoulderSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Chest) {
		ReturnSlot = EquippmentSlots.ChestSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Back) {
		ReturnSlot = EquippmentSlots.BackSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Gloves) {
		ReturnSlot = EquippmentSlots.GlovesSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Belt) {
		ReturnSlot = EquippmentSlots.BeltSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Legs) {
		ReturnSlot = EquippmentSlots.LegsSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Boots) {
		ReturnSlot = EquippmentSlots.BootsSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Ring && IsRightFinger == true) {
		ReturnSlot = EquippmentSlots.RingRightSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Ring && IsRightFinger == false) {
		ReturnSlot = EquippmentSlots.RingLeftSlot;
		return true;
	}
	else if (ArmorType == EArmorType::Bag) {
		ReturnSlot = EquippmentSlots.BagSlot;
		return true;
	}

	return false;
}

bool UP4InventoryBagComponent::SetEquipSlotInfo(EArmorType ArmorType, bool IsRightFinger, FInventoryItemStruct ItemStruct)
{
	// These hardcoded values should reflect IntiializeEmptyEquippment order
	if (ArmorType == EArmorType::Helmet) {
		EquippmentSlots.HelmetSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.HelmetSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.HelmetSlot);
		//if (EquippmentSlots.HelmetSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Helmet with " + EquippmentSlots.HelmetSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Helmet with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Necklace) {
		EquippmentSlots.NecklaceSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.NecklaceSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.NecklaceSlot);
		//if (EquippmentSlots.NecklaceSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Necklace with " + EquippmentSlots.NecklaceSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Necklace with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Shoulder) {
		EquippmentSlots.ShoulderSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.ShoulderSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.ShoulderSlot);
		//if (EquippmentSlots.ShoulderSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Shoulder with " + EquippmentSlots.ShoulderSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Shoulder with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Chest) {
		EquippmentSlots.ChestSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.ChestSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.ChestSlot);
		//if (EquippmentSlots.ChestSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Chest with " + EquippmentSlots.ChestSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Chest with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Back) {
		EquippmentSlots.BackSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BackSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BackSlot);
		//if (EquippmentSlots.BackSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Back with " + EquippmentSlots.BackSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Back with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Gloves) {
		EquippmentSlots.GlovesSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.GlovesSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.GlovesSlot);
		//if (EquippmentSlots.GlovesSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Gloves with " + EquippmentSlots.GlovesSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Gloves with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Belt) {
		EquippmentSlots.BeltSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BeltSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BeltSlot);
		//if (EquippmentSlots.BeltSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Belt with " + EquippmentSlots.BeltSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Belt with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Legs) {
		EquippmentSlots.LegsSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.LegsSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.LegsSlot);
		//if (EquippmentSlots.LegsSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Legs with " + EquippmentSlots.LegsSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Legs with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Boots) {
		EquippmentSlots.BootsSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BootsSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BootsSlot);
		//if (EquippmentSlots.BootsSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Boots with " + EquippmentSlots.BootsSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Boots with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Ring && IsRightFinger == true) {
		EquippmentSlots.RingRightSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.RingRightSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.RingRightSlot);
		//if (EquippmentSlots.RingRightSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set RingRight with " + EquippmentSlots.RingRightSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set RingRight with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Ring && IsRightFinger == false) {
		EquippmentSlots.RingLeftSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.RingLeftSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.RingLeftSlot);
		//if (EquippmentSlots.RingLeftSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set RingLeft with " + EquippmentSlots.RingLeftSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set RingLeft with Empty"));
		return true;
	}
	else if (ArmorType == EArmorType::Bag) {
		EquippmentSlots.BagSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BagSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BagSlot);
		//if (EquippmentSlots.BagSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Bag with " + EquippmentSlots.BagSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Bag with Empty"));
		return true;
	}

	print(FString("Faield to Equip item due to invalid armortype"));
	return false;
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

void UP4InventoryBagComponent::ClientBroadcastInventoryUpdateDelegate_Implementation(const FInventoryItemStruct& NewItemInfo, int Index)
{
	OnInventorySlotUpdated.Broadcast(NewItemInfo, Index);
}

void UP4InventoryBagComponent::ClientBroadcastEquippmentUpdateDelegate_Implementation(const FEquippmentSlotStruct& NewEquippmentInfo)
{
	OnEquippmentSlotUpdated.Broadcast(NewEquippmentInfo);
}

void UP4InventoryBagComponent::ServerAddItemToInventory_Implementation(const FInventoryItemStruct& NewItem, AActor* Instigator)
{
	bool WasSucessful;

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
				ClientBroadcastInventoryUpdateDelegate(NewItem, idx);

				// Check overflow
				if (InventoryArray[idx].StackCount > InventoryArray[idx].ItemBaseDataAsset->ItemInfo.MaxStackCount)
				{
					StacksToFill = InventoryArray[idx].StackCount - InventoryArray[idx].ItemBaseDataAsset->ItemInfo.MaxStackCount;
					InventoryArray[idx].StackCount = InventoryArray[idx].ItemBaseDataAsset->ItemInfo.MaxStackCount;
				}
				if (StacksToFill == 0)
					break;
			}
			// delete item
			Instigator->Destroy();
		}
	}
	else
	{
		// not stackable, place in first available slot, else return unsuccessful
		int Index; 
		FindFirstEmptySpot(WasSucessful, Index);
		if (WasSucessful)
		{
			InventoryArray.Insert(NewItem, Index);
			OnInventorySlotUpdated.Broadcast(NewItem, Index);
			ClientBroadcastInventoryUpdateDelegate(NewItem, Index);

			// delete item
			Instigator->Destroy();
		}
	}
}


void UP4InventoryBagComponent::ServerDropItemFromInventory_Implementation(int InventoryIndex)
{
	FInventoryItemStruct Item = InventoryArray[InventoryIndex];
	
	if (!Item.bIsEmpty && Item.ItemBaseDataAsset)
	{
		UClass* ItemClass =Item.ItemBaseDataAsset->ItemInfo.ItemClass;

		if (ItemClass)
		{
			const FVector Location = FVector(GetOwner()->GetActorLocation());
			const FRotator Rotation = FRotator(GetOwner()->GetActorRotation());
			const FActorSpawnParameters SpawnInfo;

			if (Item.ItemBaseDataAsset->ItemInfo.ItemStaticMesh)
			{
				print(FString(Item.ItemBaseDataAsset->ItemInfo.ItemStaticMesh->GetName()));
			}
			
			// Defer/ delay spawn until inventory Item info is transfered, then call gameplaystatics to finish spawn
			AP4ItemBaseActor* result = GetWorld()->SpawnActorDeferred<AP4ItemBaseActor>(ItemClass, GetOwner()->GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);// Rotation, nullActor, false, false, nullActor, Cast<APawn>(GetOwner()), true, GetOwner()->GetLevel(), true);
			if (result)
			{
				AP4ItemBaseActor* ItemBase = Cast<AP4ItemBaseActor>(result);
				if(ItemBase)
				{
					// Setup Mesh to spawn from item info
					ItemBase->SetItemStructAndStaticMesh(Item);
					// Spawn Actor
					UGameplayStatics::FinishSpawningActor(ItemBase, GetOwner()->GetTransform());

					InventoryArray[InventoryIndex] = FInventoryItemStruct();
					OnInventorySlotUpdated.Broadcast(FInventoryItemStruct(), InventoryIndex);
					ClientBroadcastInventoryUpdateDelegate(FInventoryItemStruct(), InventoryIndex);
				}
			}
		}
		
	}
	
}


void UP4InventoryBagComponent::ServerEquipItemFromInventory_Implementation(int InventoryIndex, bool IsRightFinger)
{
	// grab item, check that we can wear it (armor / weapon only)
	FInventoryItemStruct Item = InventoryArray[InventoryIndex];

	if (!Item.bIsEmpty && Item.ItemBaseDataAsset)
	{
		if (Item.ItemBaseDataAsset->ItemInfo.ItemType == EItemType::Armor)
		{
			// figure out what slot this goes to 
			UItemArmorDataAsset* ArmorItem = Cast<UItemArmorDataAsset>(Item.ItemBaseDataAsset);
			if (ArmorItem)
			{
				FEquippmentSlotStruct EquipSlot; // Just sentinel value, following function bool will help
				if (GetEquipSlotInfo(ArmorItem->ArmorType, IsRightFinger, EquipSlot))
				{
					if (EquipSlot.InventoryItemStruct.bIsEmpty)
					{
						SetEquipSlotInfo(ArmorItem->ArmorType, IsRightFinger, Item);

						//print(FString("Equip Item: " + ArmorItem->ItemInfo.ItemName.ToString()));

						// place empty inventory spot
						InventoryArray[InventoryIndex] = FInventoryItemStruct();
						OnInventorySlotUpdated.Broadcast(FInventoryItemStruct(), InventoryIndex);
						ClientBroadcastInventoryUpdateDelegate(FInventoryItemStruct(), InventoryIndex);

						// done here
						return;
					}
					else
					{
						//TODO: Swap items here
					}
				}	
			}
		}
		else if (Item.ItemBaseDataAsset->ItemInfo.ItemType == EItemType::Weapon)
		{
			// TODO: equip weapon implementation
		}
	}
}


				
void UP4InventoryBagComponent::ServerUnEquipItemFromInventory_Implementation(EArmorType ArmorType, bool IsRightFinger)
{

	FEquippmentSlotStruct EquipSlot;
	if (GetEquipSlotInfo(ArmorType, IsRightFinger, EquipSlot))
	{
		if (!EquipSlot.InventoryItemStruct.bIsEmpty)
		{
			bool success; int Index;
			FindFirstEmptySpot(success, Index);

			if (success)
			{
				//print(FString("UnEquip Item: " + EquipSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName.ToString()));

				// place empty inventory spot
				InventoryArray[Index] = FInventoryItemStruct(EquipSlot.InventoryItemStruct);
				OnInventorySlotUpdated.Broadcast(InventoryArray[Index], Index);
				ClientBroadcastInventoryUpdateDelegate(InventoryArray[Index], Index);

				SetEquipSlotInfo(ArmorType, IsRightFinger, FInventoryItemStruct());

				// done here
				return;

			}
			else
			{
				// TODO: send inventory full message
			}

		}
	}
}

void UP4InventoryBagComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//SetIsReplicated(true);
}

void UP4InventoryBagComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(UP4InventoryBagComponent, InventoryArray);
	DOREPLIFETIME(UP4InventoryBagComponent, EquippmentSlots);
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




