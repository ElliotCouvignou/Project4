// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4InventoryBagComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"

#include "Characters/Project4Character.h"
#include "Interactables/P4ItemBaseActor.h"
#include "Interactables/ItemArmorDataAsset.h"

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

	WeaponStance = EWeaponStanceType::MeleeDualWield; // unarmed i guess 

	// TODO: push empty weps here
	//EquippmentArray.Push(FEquippmentSlotStruct(EArmorType::Helmet));
	//EquippmentArray.Push(FEquippmentSlotStruct(EArmorType::Helmet));

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

void UP4InventoryBagComponent::GetWeaponTypes(EWeaponType& MainWeaponType, EWeaponType& OffWeaponType)
{
	UItemWeaponDataAsset* WeaponDataAsset = Cast<UItemWeaponDataAsset>(EquippmentSlots.WeaponRightSlot.InventoryItemStruct.ItemBaseDataAsset);
	if (WeaponDataAsset)
		MainWeaponType = WeaponDataAsset->WeaponType;
	else
		MainWeaponType = EWeaponType::None;

	WeaponDataAsset = Cast<UItemWeaponDataAsset>(EquippmentSlots.WeaponLeftSlot.InventoryItemStruct.ItemBaseDataAsset);
	if (WeaponDataAsset)
		OffWeaponType = WeaponDataAsset->WeaponType;
	else
		OffWeaponType = EWeaponType::None;

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


void UP4InventoryBagComponent::ServerEquipItemFromInventory_Implementation(int InventoryIndex, bool IsRightSide)
{
	// grab item, check that we can wear it (armor / weapon only)
	FInventoryItemStruct Item = InventoryArray[InventoryIndex];

	if (!Item.bIsEmpty && Item.ItemBaseDataAsset)
	{
		if (Item.ItemBaseDataAsset->ItemInfo.ItemType == EItemType::Armor)
		{
			EquipArmorItemFromInventory(InventoryIndex, IsRightSide, Item);
		}
		else if (Item.ItemBaseDataAsset->ItemInfo.ItemType == EItemType::Weapon)
		{
			EquipWeaponItemFromInventory(InventoryIndex, IsRightSide, Item);
		}
	}
}

void UP4InventoryBagComponent::EquipArmorItemFromInventory(int InventoryIndex, bool IsRightFinger, FInventoryItemStruct& Item)
{
	// figure out what slot this goes to 
	const UItemArmorDataAsset* ArmorItem = Cast<UItemArmorDataAsset>(Item.ItemBaseDataAsset);
	if (ArmorItem)
	{
		FEquippmentSlotStruct EquipSlot; // Just sentinel value, following function bool will help
		EEquipSlotType EquipSlotType = ArmorTypeToEquipSlotType(ArmorItem->ArmorType, IsRightFinger);
		if (GetEquipSlotInfo(EquipSlotType, EquipSlot))
		{
			if (EquipSlot.InventoryItemStruct.bIsEmpty)
			{
				AProject4Character* P4Char = Cast<AProject4Character>(GetOwner());
				if (P4Char)
				{
					UAbilitySystemComponent* ASC = P4Char->GetAbilitySystemComponent();
					if (ASC)
					{
						FActiveGameplayEffectHandle EqippedEffectHandle = ASC->BP_ApplyGameplayEffectToSelf(ArmorItem->EquippedGameplayEffect, 1, ASC->MakeEffectContext());
						if (EqippedEffectHandle.WasSuccessfullyApplied())
						{
							// place empty inventory spot
							InventoryArray[InventoryIndex] = FInventoryItemStruct();
							OnInventorySlotUpdated.Broadcast(FInventoryItemStruct(), InventoryIndex);
							ClientBroadcastInventoryUpdateDelegate(FInventoryItemStruct(), InventoryIndex);

							SetEquipSlotInfo(EquipSlotType, Item, EqippedEffectHandle);

							// done here
							return;
						}
						else
						{
							print(FString("Invalid GE equip applying :(0"));
						}
					}
				}
			}
			else
			{
				//TODO: Swap items here
			}
		}
	}
}


void UP4InventoryBagComponent::EquipWeaponItemFromInventory(int InventoryIndex, bool IsRightHand, FInventoryItemStruct& Item)
{
	// TODO: add method for weapon types and replacement of such
	const UItemWeaponDataAsset* WeaponItem = Cast<UItemWeaponDataAsset>(Item.ItemBaseDataAsset);
	if (WeaponItem)
	{
		FEquippmentSlotStruct EquipSlot;
		EEquipSlotType EquipSlotType = (IsRightHand) ? EEquipSlotType::WeaponRight : EEquipSlotType::WeaponLeft;
		if (GetEquipSlotInfo(EquipSlotType, EquipSlot))
		{
			if (EquipSlot.InventoryItemStruct.bIsEmpty)
			{
				AProject4Character* P4Char = Cast<AProject4Character>(GetOwner());
				if (P4Char)
				{
					UAbilitySystemComponent* ASC = P4Char->GetAbilitySystemComponent();
					if (ASC)
					{
						FActiveGameplayEffectHandle EqippedEffectHandle = ASC->BP_ApplyGameplayEffectToSelf(WeaponItem->EquippedGameplayEffect, 1, ASC->MakeEffectContext());
						if (EqippedEffectHandle.WasSuccessfullyApplied())
						{
							// place empty inventory spot
							InventoryArray[InventoryIndex] = FInventoryItemStruct();
							OnInventorySlotUpdated.Broadcast(FInventoryItemStruct(), InventoryIndex);
							ClientBroadcastInventoryUpdateDelegate(FInventoryItemStruct(), InventoryIndex);

							SetEquipSlotInfo(EquipSlotType, Item, EqippedEffectHandle);

							SetNewWeaponStanceFromWeapon(WeaponItem);

							if (IsRightHand)
								P4Char->SetRightHandWeaponInfo(WeaponItem);
							else
								P4Char->SetLeftHandWeaponInfo(WeaponItem);
							

							// done here
							return;
						}
						else
						{
							print(FString("Invalid GE equip applying :(0"));
						}
					}
				}
			}
			else
			{
				//TODO: Swap items here
			}
		}
	}

}




				
void UP4InventoryBagComponent::ServerUnEquipItemFromInventory_Implementation(EEquipSlotType EquipSlotType)
{

	FEquippmentSlotStruct EquipSlot;
	if (GetEquipSlotInfo(EquipSlotType, EquipSlot))
	{
		if (!EquipSlot.InventoryItemStruct.bIsEmpty)
		{
			bool success; int Index;
			FindFirstEmptySpot(success, Index);

			if (success)
			{
				//print(FString("UnEquip Item: " + EquipSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName.ToString()));
				AProject4Character* P4Char = Cast<AProject4Character>(GetOwner());
				if (P4Char)
				{
					UAbilitySystemComponent* ASC = P4Char->GetAbilitySystemComponent();
					if (ASC)
					{
						if (ASC->RemoveActiveGameplayEffect(EquipSlot.EqippedEffectHandle))
						{
							// place empty inventory spot
							InventoryArray[Index] = FInventoryItemStruct(EquipSlot.InventoryItemStruct);
							OnInventorySlotUpdated.Broadcast(InventoryArray[Index], Index);
							ClientBroadcastInventoryUpdateDelegate(InventoryArray[Index], Index);

							SetEquipSlotInfo(EquipSlotType, FInventoryItemStruct(), FActiveGameplayEffectHandle());

							if (EquipSlotType == EEquipSlotType::WeaponRight)
								P4Char->SetRightHandWeaponInfo(nullptr);
							else if (EquipSlotType == EEquipSlotType::WeaponLeft)
								P4Char->SetLeftHandWeaponInfo(nullptr);

							// done here
							return;
						}
						else
						{
							print(FString("failed to remove active GE on unequip :("));
						}
					}
				}
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
	DOREPLIFETIME(UP4InventoryBagComponent, WeaponStance);
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

void UP4InventoryBagComponent::SetNewWeaponStanceFromWeapon(const UItemWeaponDataAsset* WeaponItem)
{
	if (WeaponItem)
	{
		// query weapon types before hand types, check handtypes for types that could be multiple handtypes
		// Startoff with ones that dont need to query handtypes
		switch (WeaponItem->WeaponType)
		{
		case EWeaponType::Sword:
			if (WeaponItem->WeaponHandType == EWeaponHandType::BothHands)
				WeaponStance = EWeaponStanceType::MeleeMainHandOnly;
			else
				WeaponStance = EWeaponStanceType::MeleeDualWield;
			break;
		case EWeaponType::Axe:
			if (WeaponItem->WeaponHandType == EWeaponHandType::BothHands)
				WeaponStance = EWeaponStanceType::MeleeMainHandOnly;
			else
				WeaponStance = EWeaponStanceType::MeleeDualWield;
			break;
		case EWeaponType::Gun:
			if (WeaponItem->WeaponHandType == EWeaponHandType::BothHands)
				WeaponStance = EWeaponStanceType::RangedMainHandOnly;
			else
				WeaponStance = EWeaponStanceType::RangedDualWield;
			break;
		case EWeaponType::Crossbow:
			if (WeaponItem->WeaponHandType == EWeaponHandType::BothHands)
				WeaponStance = EWeaponStanceType::RangedMainHandOnly;
			else
				WeaponStance = EWeaponStanceType::RangedDualWield;
			break;
		case EWeaponType::Dagger:
			WeaponStance = EWeaponStanceType::MeleeDualWield;
			break;
		case EWeaponType::Wand:
			WeaponStance = EWeaponStanceType::RangedDualWield;
			break;
		case EWeaponType::Orb:
			WeaponStance = EWeaponStanceType::RangedMainHandOnly;  // orbs dont have an attack, they buff other wep
			break;
		case EWeaponType::Shield:
			WeaponStance = EWeaponStanceType::MeleeMainHandOnly;
			break;
		case EWeaponType::Quiver:
			WeaponStance = EWeaponStanceType::RangedMainHandOnly;
			break;
		case EWeaponType::Staff:
			WeaponStance = EWeaponStanceType::RangedMainHandOnly;
			break;
		case EWeaponType::Bow:
			WeaponStance = EWeaponStanceType::RangedMainHandOnly;
			break;


		default:
			WeaponStance = EWeaponStanceType::None;
			break;
		}
	}

}

EEquipSlotType UP4InventoryBagComponent::ArmorTypeToEquipSlotType(EArmorType ArmorType, bool IsRightFinger)
{
	if (ArmorType == EArmorType::Ring && IsRightFinger)
		return EEquipSlotType::RingRight;
	else if (ArmorType == EArmorType::Necklace && !IsRightFinger)
		return EEquipSlotType::RingLeft;

	switch (ArmorType)
	{
	case EArmorType::Helmet:
		return EEquipSlotType::Helmet;
		break;
	case EArmorType::Necklace:
		return EEquipSlotType::Necklace;
		break;
	case EArmorType::Shoulder:
		return EEquipSlotType::Shoulder;
		break;
	case EArmorType::Chest:
		return EEquipSlotType::Chest;
		break;
	case EArmorType::Back:
		return EEquipSlotType::Back;
		break;
	case EArmorType::Gloves:
		return EEquipSlotType::Gloves;
		break;
	case EArmorType::Belt:
		return EEquipSlotType::Belt;
		break;
	case EArmorType::Legs:
		return EEquipSlotType::Legs;
		break;
	case EArmorType::Boots:
		return EEquipSlotType::Boots;
		break;
	case EArmorType::Bag:
		return EEquipSlotType::Bag;
		break;

	default:
		return EEquipSlotType::None;
	}

	return EEquipSlotType::None;
}

bool UP4InventoryBagComponent::GetEquipSlotInfo(EEquipSlotType EquipSlotType, FEquippmentSlotStruct& ReturnSlot)
{
	// These hardcoded values should reflect IntiializeEmptyEquippment order
	switch (EquipSlotType)
	{
	case EEquipSlotType::Helmet:
		ReturnSlot = EquippmentSlots.HelmetSlot;
		return true;
	case EEquipSlotType::Necklace:
		ReturnSlot = EquippmentSlots.NecklaceSlot;
		return true;
	case EEquipSlotType::Shoulder:
		ReturnSlot = EquippmentSlots.ShoulderSlot;
		return true;
	case EEquipSlotType::Chest:
		ReturnSlot = EquippmentSlots.ChestSlot;
		return true;
	case EEquipSlotType::Back:
		ReturnSlot = EquippmentSlots.BackSlot;
		return true;
	case EEquipSlotType::Gloves:
		ReturnSlot = EquippmentSlots.GlovesSlot;
		return true;
	case EEquipSlotType::Belt:
		ReturnSlot = EquippmentSlots.BeltSlot;
		return true;
	case EEquipSlotType::Legs:
		ReturnSlot = EquippmentSlots.LegsSlot;
		return true;
	case EEquipSlotType::Boots:
		ReturnSlot = EquippmentSlots.BootsSlot;
		return true;
	case EEquipSlotType::Bag:
		ReturnSlot = EquippmentSlots.BagSlot;
		return true;
	case EEquipSlotType::RingRight:
		ReturnSlot = EquippmentSlots.RingRightSlot;
		return true;
	case EEquipSlotType::RingLeft:
		ReturnSlot = EquippmentSlots.RingLeftSlot;
		return true;
	case EEquipSlotType::WeaponRight:
		ReturnSlot = EquippmentSlots.WeaponRightSlot;
		return true;
	case EEquipSlotType::WeaponLeft:
		ReturnSlot = EquippmentSlots.WeaponLeftSlot;
		return true;

	default:
		return false;
	}

	return false;
}

bool UP4InventoryBagComponent::SetEquipSlotInfo(EEquipSlotType EquipSlotType, FInventoryItemStruct ItemStruct, FActiveGameplayEffectHandle ActiveGEHandle)
{
	switch (EquipSlotType)
	{
	case EEquipSlotType::None:
		return false;
	case EEquipSlotType::Helmet:
		EquippmentSlots.HelmetSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		EquippmentSlots.HelmetSlot.EqippedEffectHandle = ActiveGEHandle;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.HelmetSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.HelmetSlot);
		//if (EquippmentSlots.HelmetSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Helmet with " + EquippmentSlots.HelmetSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Helmet with Empty"));
		return true;
		break;
	case EEquipSlotType::Necklace:
		EquippmentSlots.NecklaceSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.NecklaceSlot);
		EquippmentSlots.NecklaceSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.NecklaceSlot);
		//if (EquippmentSlots.NecklaceSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Necklace with " + EquippmentSlots.NecklaceSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Necklace with Empty"));
		return true;
		break;
	case EEquipSlotType::Shoulder:
		EquippmentSlots.ShoulderSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.ShoulderSlot);
		EquippmentSlots.ShoulderSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.ShoulderSlot);
		//if (EquippmentSlots.ShoulderSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Shoulder with " + EquippmentSlots.ShoulderSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Shoulder with Empty"));
		return true;
		break;
	case EEquipSlotType::Chest:
		EquippmentSlots.ChestSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.ChestSlot);
		EquippmentSlots.ChestSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.ChestSlot);
		//if (EquippmentSlots.ChestSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Chest with " + EquippmentSlots.ChestSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Chest with Empty"));
		return true;
		break;
	case EEquipSlotType::Back:
		EquippmentSlots.BackSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BackSlot);
		EquippmentSlots.BackSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BackSlot);
		//if (EquippmentSlots.BackSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Back with " + EquippmentSlots.BackSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Back with Empty"));
		return true;
		break;
	case EEquipSlotType::Gloves:
		EquippmentSlots.GlovesSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.GlovesSlot);
		EquippmentSlots.GlovesSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.GlovesSlot);
		//if (EquippmentSlots.GlovesSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Gloves with " + EquippmentSlots.GlovesSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Gloves with Empty"));
		return true;
		break;
	case EEquipSlotType::Belt:
		EquippmentSlots.BeltSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BeltSlot);
		EquippmentSlots.BeltSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BeltSlot);
		//if (EquippmentSlots.BeltSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Belt with " + EquippmentSlots.BeltSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Belt with Empty"));
		return true;
		break;
	case EEquipSlotType::Legs:
		EquippmentSlots.LegsSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.LegsSlot);
		EquippmentSlots.LegsSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.LegsSlot);
		//if (EquippmentSlots.LegsSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Legs with " + EquippmentSlots.LegsSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Legs with Empty"));
		return true;
		break;
	case EEquipSlotType::Boots:
		EquippmentSlots.BootsSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BootsSlot);
		EquippmentSlots.BootsSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BootsSlot);
		//if (EquippmentSlots.BootsSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Boots with " + EquippmentSlots.BootsSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Boots with Empty"));
		return true;
		break;
	case EEquipSlotType::RingLeft:
		EquippmentSlots.RingRightSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.RingRightSlot);
		EquippmentSlots.RingRightSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.RingRightSlot);
		//if (EquippmentSlots.RingRightSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set RingRight with " + EquippmentSlots.RingRightSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set RingRight with Empty"));
		return true;
		break;
	case EEquipSlotType::RingRight:
		EquippmentSlots.RingLeftSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.RingLeftSlot);
		EquippmentSlots.RingLeftSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.RingLeftSlot);
		//if (EquippmentSlots.RingLeftSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set RingLeft with " + EquippmentSlots.RingLeftSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set RingLeft with Empty"));
		return true;
		break;
	case EEquipSlotType::Bag:
		EquippmentSlots.BagSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BagSlot);
		EquippmentSlots.BagSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BagSlot);
		//if (EquippmentSlots.BagSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set Bag with " + EquippmentSlots.BagSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set Bag with Empty"));
		return true;
		break;
	case EEquipSlotType::WeaponLeft:
		EquippmentSlots.WeaponLeftSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.WeaponLeftSlot);
		EquippmentSlots.WeaponLeftSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.WeaponLeftSlot);
		//if (EquippmentSlots.WeaponLeftSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set WeaponLeft with " + EquippmentSlots.WeaponLeftSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set WeaponLeft with Empty"));
		return true;
		break;
	case EEquipSlotType::WeaponRight:
		EquippmentSlots.WeaponRightSlot.InventoryItemStruct = FInventoryItemStruct(ItemStruct);
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.WeaponRightSlot);
		EquippmentSlots.WeaponRightSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.WeaponRightSlot);
		//if (EquippmentSlots.WeaponRightSlot.InventoryItemStruct.ItemBaseDataAsset)
		//	print(FString("Set WeaponRight with " + EquippmentSlots.WeaponRightSlot.InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName));
		//else
		//	print(FString("Set WeaponRight with Empty"));
		return true;
		break;

	default:
		return false;
	}

	print(FString("Faield to Equip item due to invalid EquipSlotType"));
	return false;
}



