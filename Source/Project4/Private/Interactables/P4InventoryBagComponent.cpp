// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4InventoryBagComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"

#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Characters/Project4Character.h"
#include "Interactables/P4ItemBaseActor.h"
#include "Interactables/ItemArmorDataAsset.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "Interactables/P4ItemWeaponObject.h"
#include "Interactables/P4ItemArmorObject.h"


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

	// might not work cause race cond. but why not
	GetSetPlayerRefAndASC();
}

void UP4InventoryBagComponent::GetSetPlayerRefAndASC()
{
	// try get Player and ASC
	if (!PlayerRef)
	{
		PlayerRef = Cast<AProject4Character>(GetOwner());
	}
	if (!PlayerASC)
	{
		PlayerASC = Cast<UAbilitySystemComponent>(PlayerRef->GetAbilitySystemComponent());
	}
}


void UP4InventoryBagComponent::FindFirstEmptySpot(bool& WasSuccessful, int& Index)
{
	WasSuccessful = false;

	int idx = 0;
	for (UP4ItemBaseObject* Item : InventoryArray)
	{
		if (!Item || Item->bIsEmpty)
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
		InventoryArray[i] = nullptr;
	}
}

void UP4InventoryBagComponent::InitializeEmptyEquippment()
{
	EquippmentSlots = FEquippedItemsStruct(); 
}



void UP4InventoryBagComponent::FindItemsFromItemData(const UP4ItemBaseObject* SearchItem, bool& WasFound, TArray<int>& FoundIndexes)
{
	WasFound = false;

	int idx = 0;
	for (UP4ItemBaseObject* Item : InventoryArray)
	{
		if (Item && Item->ItemName == SearchItem->ItemName)
		{
			WasFound = true;
			FoundIndexes.Push(idx);
		}
		idx++;
	}
	
}

void UP4InventoryBagComponent::GetWeaponTypes(EWeaponType& MainWeaponType, EWeaponType& OffWeaponType)
{
	// TODO: make sure new weapon UObject items can work here
	UP4ItemWeaponObject* WeaponDataAsset = Cast<UP4ItemWeaponObject>(EquippmentSlots.WeaponRightSlot.InventoryItemObject);
	if (WeaponDataAsset)
		MainWeaponType = WeaponDataAsset->WeaponType;
	else
		MainWeaponType = EWeaponType::None;

	WeaponDataAsset = Cast<UP4ItemWeaponObject>(EquippmentSlots.WeaponLeftSlot.InventoryItemObject);
	if (WeaponDataAsset)
		OffWeaponType = WeaponDataAsset->WeaponType;
	else
		OffWeaponType = EWeaponType::None;

}


void UP4InventoryBagComponent::ClientBroadcastInventoryUpdateDelegate_Implementation(int Index, const UP4ItemBaseObject* ItemInfoObject)
{
	OnInventorySlotUpdated.Broadcast(Index, ItemInfoObject);
}

void UP4InventoryBagComponent::ClientBroadcastEquippmentUpdateDelegate_Implementation(const FEquippmentSlotStruct& NewEquippmentInfo)
{
	OnEquippmentSlotUpdated.Broadcast(NewEquippmentInfo);
}

void UP4InventoryBagComponent::ServerAddItemToInventory_Implementation(UP4ItemBaseObject* NewItem, AActor* Instigator)
{
	if(!NewItem)
	{
		return;
	}
	bool WasSucessful;
	//UItemBaseDataAsset* ItemDataAsset = (NewItem.ItemBaseDataAsset) ? NewItem.ItemBaseDataAsset : NewItem.ItemInfoStruct.
	//const FItemBaseInfoStruct& ItemInfoStruct = (NewItem->ItemBaseDataAsset) ? NewItem->ItemBaseDataAsset->ItemInfo : NewItem.ItemInfoStruct;
 	if (NewItem->bIsStackable)
	{
		// Stackable, find locations that exists in inventory and stack (if max reached find next stack, if no stacks then do same approach as nonstackable (first available spot ))
		TArray<int> FoundIdxes;
		FindItemsFromItemData(NewItem, WasSucessful, FoundIdxes);
		if (WasSucessful)
		{
			int StacksToFill = NewItem->StackCount;
			for (int idx : FoundIdxes)
			{
				//Stack Items
				InventoryArray[idx]->StackCount += StacksToFill;
				OnInventorySlotUpdated.Broadcast(idx, InventoryArray[idx]);
				ClientBroadcastInventoryUpdateDelegate(idx, InventoryArray[idx]);

				// TODO: add weight, maybe fix this bit as it doesnt really make sense intuitively 

				// Check overflow
				if (InventoryArray[idx]->StackCount > InventoryArray[idx]->MaxStackCount)
				{
					StacksToFill = InventoryArray[idx]->StackCount - InventoryArray[idx]->MaxStackCount;
					InventoryArray[idx]->StackCount = InventoryArray[idx]->MaxStackCount;
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
			// add weight
			FGameplayEffectSpecHandle PickupEffectSpec = PlayerASC->MakeOutgoingSpec(ItemPickupOrDropGE, 1, PlayerASC->MakeEffectContext());
			if (PickupEffectSpec.IsValid())
			{
				FGameplayEffectSpec* GESpec = PickupEffectSpec.Data.Get();
				FGameplayTag CarryWeight = FGameplayTag::RequestGameplayTag(FName("Data.Attribute.CarryWeight"));
				GESpec->SetSetByCallerMagnitude(CarryWeight, NewItem->ItemWeight);

				InventoryArray.Insert(NewItem, Index);
				//OnInventorySlotUpdated.Broadcast(Index, NewItem);
				ClientBroadcastInventoryUpdateDelegate(Index, NewItem);

				InventoryArray[Index]->ActiveGE = PlayerASC->ApplyGameplayEffectSpecToTarget(*PickupEffectSpec.Data.Get(), PlayerASC);

				// delete item
				Instigator->Destroy();
			}
		}
	}
}


void UP4InventoryBagComponent::ServerDropItemFromInventory_Implementation(int InventoryIndex)
{
	UP4ItemBaseObject* Item = InventoryArray[InventoryIndex];
	if (Item && !Item->bIsEmpty)
	{
		UClass* ItemClass = Item->DroppedItemClass;

		if (ItemClass)
		{
			const FVector Location = FVector(GetOwner()->GetActorLocation()); 
			const FRotator Rotation = FRotator(GetOwner()->GetActorRotation());
			const FActorSpawnParameters SpawnInfo;
			
			// Defer/ delay spawn until inventory Item info is transfered, then call gameplaystatics to finish spawn
			AP4ItemBaseActor* result = GetWorld()->SpawnActorDeferred<AP4ItemBaseActor>(ItemClass, GetOwner()->GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);// Rotation, nullActor, false, false, nullActor, Cast<APawn>(GetOwner()), true, GetOwner()->GetLevel(), true);
			if (result)
			{
				AP4ItemBaseActor* ItemBase = Cast<AP4ItemBaseActor>(result);
				if(ItemBase)
				{
					// Setup Mesh to spawn from item info
					ItemBase->SetInventoryItemObject(&Item);
					// Spawn Actor
					ItemBase->FinishSpawning(GetOwner()->GetTransform());

					// remove added weight GE
					PlayerASC->RemoveActiveGameplayEffect(InventoryArray[InventoryIndex]->ActiveGE, 1);

					InventoryArray[InventoryIndex] = nullptr;
					OnInventorySlotUpdated.Broadcast(InventoryIndex, InventoryArray[InventoryIndex]);
					ClientBroadcastInventoryUpdateDelegate(InventoryIndex, InventoryArray[InventoryIndex]);
				}
			}
		}	
	}
}


void UP4InventoryBagComponent::ServerEquipItemFromInventory_Implementation(int InventoryIndex, bool IsRightSide)
{
	// grab item, check that we can wear it (armor / weapon only)
	UP4ItemBaseObject* Item = InventoryArray[InventoryIndex];

	if (Item && !Item->bIsEmpty)
	{
		if (Item->ItemType == EItemType::Armor)
		{
			EquipArmorItemFromInventory(InventoryIndex, IsRightSide);
		}
		else if (Item->ItemType == EItemType::Weapon)
		{
			EquipWeaponItemFromInventory(InventoryIndex, IsRightSide);
		}
	}
}

void UP4InventoryBagComponent::ServerSwapInventoryItems_Implementation(int DragStartIndex, int DragEndIndex)
{
	UP4ItemBaseObject* StartItem = InventoryArray[DragStartIndex];
	UP4ItemBaseObject* EndItem = InventoryArray[DragEndIndex];

	if (!StartItem || StartItem->bIsEmpty)
	{
		UP4ItemBaseObject* temp = StartItem;
		InventoryArray[DragStartIndex] = EndItem;
		InventoryArray[DragEndIndex] = temp;
		ClientBroadcastInventoryUpdateDelegate(DragStartIndex, InventoryArray[DragStartIndex]);
		ClientBroadcastInventoryUpdateDelegate(DragEndIndex, InventoryArray[DragEndIndex]);
	}
}

void UP4InventoryBagComponent::EquipArmorItemFromInventory(int InventoryIndex, bool IsRightFinger)
{
	// figure out what slot this goes to 
	UP4ItemArmorObject* ArmorItem = Cast<UP4ItemArmorObject>(InventoryArray[InventoryIndex]);
	if (ArmorItem)
	{
		FEquippmentSlotStruct EquipSlot;
		EEquipSlotType EquipSlotType = ArmorTypeToEquipSlotType(ArmorItem->ArmorType, IsRightFinger);
		if (GetEquipSlotInfo(EquipSlotType, EquipSlot))
		{
			// Check for swap case, place equipped item in inventory
			InventoryArray[InventoryIndex] = nullptr;
			if (EquipSlot.InventoryItemObject && !EquipSlot.InventoryItemObject->bIsEmpty)
			{
				PlayerASC->RemoveActiveGameplayEffect(EquipSlot.EqippedEffectHandle);
				InventoryArray[InventoryIndex] = EquipSlot.InventoryItemObject;
			}

			// Apply GE and set equip slot info, if both succeed then broadcast
			if (ArmorItem->EquippedGameplayEffect.Data.IsValid())
			{
				ArmorItem->ActiveGE = PlayerASC->ApplyGameplayEffectSpecToSelf(*ArmorItem->EquippedGameplayEffect.Data.Get());
			}
			else if (ArmorItem->PremadeGameplayEffectClass && !ArmorItem->ActiveGE.IsValid())
			{
				UGameplayEffect* GameplayEffect = ArmorItem->PremadeGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
				if (GameplayEffect)
					ArmorItem->ActiveGE = PlayerASC->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, PlayerASC->MakeEffectContext());
			}
			
			if (ArmorItem->ActiveGE.WasSuccessfullyApplied() && SetEquipSlotInfo(EquipSlotType, (UP4ItemBaseObject*)(ArmorItem), ArmorItem->ActiveGE))
			{
				OnInventorySlotUpdated.Broadcast(InventoryIndex, InventoryArray[InventoryIndex]);
				ClientBroadcastInventoryUpdateDelegate(InventoryIndex, InventoryArray[InventoryIndex]);

				// done here
				return;
			}
			else
			{
				print(FString("Failed equip armor GE"));
			}
		}
	}
}


void UP4InventoryBagComponent::EquipWeaponItemFromInventory(int InventoryIndex, bool IsRightHand)
{
	UP4ItemWeaponObject* WeaponItem = Cast<UP4ItemWeaponObject>(InventoryArray[InventoryIndex]);
	if (WeaponItem)
	{
		FEquippmentSlotStruct OffHandEquipSlot;
		FEquippmentSlotStruct MainHandEquipSlot;

		if (GetEquipSlotInfo(EEquipSlotType::WeaponLeft, OffHandEquipSlot) && GetEquipSlotInfo(EEquipSlotType::WeaponRight, MainHandEquipSlot))
		{
			UP4ItemWeaponObject* MainWep = Cast<UP4ItemWeaponObject>(MainHandEquipSlot.InventoryItemObject);
			UP4ItemWeaponObject* OffWep = Cast<UP4ItemWeaponObject>(OffHandEquipSlot.InventoryItemObject);

			/*     UNEQUIP Weapons that are in the way..     */
			// checks and does swap depending on item handtype
			EWeaponStanceType& WeaponStance = PlayerRef->WeaponStance;
			if (WeaponStance == EWeaponStanceType::MeleeDualWield || WeaponStance == EWeaponStanceType::RangedDualWield)
			{
				//  if equipping 2h then remove both, else remove desired
				if (WeaponItem->WeaponHandType == EWeaponHandType::BothHands)
				{
					// Unequip both items
					if (MainWep && PlayerASC->RemoveActiveGameplayEffect(MainHandEquipSlot.EqippedEffectHandle))
					{
						PlayerRef->ResetRightHandWeaponInfo();
						InventoryArray[InventoryIndex] = MainHandEquipSlot.InventoryItemObject;

						bool success; int index;
						FindFirstEmptySpot(success, index);
						if (OffWep && success)
						{
							// here if need to unequip both weapons
							if (PlayerASC->RemoveActiveGameplayEffect(OffHandEquipSlot.EqippedEffectHandle))
							{
								PlayerRef->ResetLeftHandWeaponInfo();

								// broadcast changes since only time another index is relevant
								InventoryArray[index] = OffHandEquipSlot.InventoryItemObject;
								OnInventorySlotUpdated.Broadcast(index, InventoryArray[index]);
								ClientBroadcastInventoryUpdateDelegate(index, InventoryArray[index]);

								SetEquipSlotInfo(EEquipSlotType::WeaponLeft, nullptr, FActiveGameplayEffectHandle());
							}
						}
					}
					else if (OffWep && PlayerASC->RemoveActiveGameplayEffect(OffHandEquipSlot.EqippedEffectHandle))
					{
						PlayerRef->ResetLeftHandWeaponInfo();
						InventoryArray[InventoryIndex] = OffHandEquipSlot.InventoryItemObject;
					}
				}
				else {
					if (IsRightHand && MainWep)
					{
						if (PlayerASC->RemoveActiveGameplayEffect(MainHandEquipSlot.EqippedEffectHandle)) {
							PlayerRef->ResetRightHandWeaponInfo();
							InventoryArray[InventoryIndex] = MainHandEquipSlot.InventoryItemObject;
						}
					}
					else if (!IsRightHand && OffWep)
					{
						if(PlayerASC->RemoveActiveGameplayEffect(OffHandEquipSlot.EqippedEffectHandle)) {
							PlayerRef->ResetLeftHandWeaponInfo();
							InventoryArray[InventoryIndex] = OffHandEquipSlot.InventoryItemObject;
						}
					}
				}
			}
			else if ((MainWep) && (WeaponStance == EWeaponStanceType::MeleeMainHandOnly || WeaponStance == EWeaponStanceType::RangedMainHandOnly))
			{
				// just remove mainhand, do unique check for quiver equip onto bow/crossbow
				if (!((WeaponItem->WeaponType == EWeaponType::Quiver) && (MainWep->WeaponType == EWeaponType::Bow || MainWep->WeaponType == EWeaponType::Crossbow)))
				{
					if (PlayerASC->RemoveActiveGameplayEffect(MainHandEquipSlot.EqippedEffectHandle))
					{
						// remove Attribute GE
						PlayerRef->ResetRightHandWeaponInfo();
						if (!IsRightHand)
						{
							// empty mainhand slot if set some offhander to undo 2h
							SetEquipSlotInfo(EEquipSlotType::WeaponRight, nullptr, FActiveGameplayEffectHandle());
						}
						InventoryArray[InventoryIndex] = MainHandEquipSlot.InventoryItemObject;
					}
				}
			}


			/*     Equip Inputted Weapon     */
			if (WeaponItem->EquippedGameplayEffect.Data.IsValid())
			{
				WeaponItem->ActiveGE = PlayerASC->ApplyGameplayEffectSpecToSelf(*WeaponItem->EquippedGameplayEffect.Data.Get());
			}
			else if(WeaponItem->PremadeGameplayEffectClass && !WeaponItem->ActiveGE.IsValid())
			{
				UGameplayEffect* GameplayEffect = WeaponItem->PremadeGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
				if(GameplayEffect)
					WeaponItem->ActiveGE = PlayerASC->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, PlayerASC->MakeEffectContext());
			}

			if (WeaponItem->ActiveGE.WasSuccessfullyApplied())
			{
				if (IsRightHand)
					SetEquipSlotInfo(EEquipSlotType::WeaponRight, (UP4ItemBaseObject*)WeaponItem, WeaponItem->ActiveGE);
				else
					SetEquipSlotInfo(EEquipSlotType::WeaponLeft, (UP4ItemBaseObject*)WeaponItem, WeaponItem->ActiveGE);

				SetNewWeaponStanceFromWeapon(WeaponItem);

				if (IsRightHand)
					PlayerRef->SetRightHandWeaponInfo(WeaponItem);
				else
					PlayerRef->SetLeftHandWeaponInfo(WeaponItem);

				// place empty inventory spot, if we swap then this will be overwritten before delegate broadcast
				InventoryArray[InventoryIndex] = nullptr;
				OnInventorySlotUpdated.Broadcast(InventoryIndex, InventoryArray[InventoryIndex]);
				ClientBroadcastInventoryUpdateDelegate(InventoryIndex, InventoryArray[InventoryIndex]);
				// done here
				return;
			}
			else
			{
				print(FString("Invalid GE equip applying :("));
			}
		}
	}

}


				
void UP4InventoryBagComponent::ServerUnEquipItemFromInventory_Implementation(EEquipSlotType EquipSlotType)
{

	FEquippmentSlotStruct EquipSlot;
	if (GetEquipSlotInfo(EquipSlotType, EquipSlot))
	{
		if (EquipSlot.InventoryItemObject && !EquipSlot.InventoryItemObject->bIsEmpty)
		{
			bool success; int Index;
			FindFirstEmptySpot(success, Index);
			if (success)
			{
				//if (!PlayerRef || !PlayerASC)
				//{
				//	GetSetPlayerRefAndASC();
				//}
				
				// remove only one stack since we could wear two rings of same type
				if (PlayerASC->RemoveActiveGameplayEffect(EquipSlot.EqippedEffectHandle, 1))
				{
					// place empty inventory spot
					InventoryArray[Index] = EquipSlot.InventoryItemObject;
					OnInventorySlotUpdated.Broadcast(Index, InventoryArray[Index]);
					ClientBroadcastInventoryUpdateDelegate(Index, InventoryArray[Index]);

					SetEquipSlotInfo(EquipSlotType, nullptr, FActiveGameplayEffectHandle());

					if (EquipSlotType == EEquipSlotType::WeaponRight)
						PlayerRef->ResetRightHandWeaponInfo();
					else if (EquipSlotType == EEquipSlotType::WeaponLeft)
						PlayerRef->ResetLeftHandWeaponInfo();

					// done here
					return;
				}
				else
				{
					print(FString("failed to remove active GE on unequip :("));
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
	DOREPLIFETIME_CONDITION(UP4InventoryBagComponent, InventoryArray, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UP4InventoryBagComponent, EquippmentSlots, COND_OwnerOnly);
}



void UP4InventoryBagComponent::SetNewWeaponStanceFromWeapon(const UP4ItemWeaponObject* WeaponItem)
{
	EWeaponStanceType& WeaponStance = PlayerRef->WeaponStance;
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
	{
		return EEquipSlotType::RingRight;
	}
	else if (ArmorType == EArmorType::Ring && !IsRightFinger)
	{
		return EEquipSlotType::RingLeft;
	}	
	else {
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

bool UP4InventoryBagComponent::SetEquipSlotInfo(EEquipSlotType EquipSlotType, UP4ItemBaseObject* ItemObject, FActiveGameplayEffectHandle ActiveGEHandle)
{
	switch (EquipSlotType)
	{
	case EEquipSlotType::None:
		print(FString("Set equipslottype none, return false"));
		return false;
	case EEquipSlotType::Helmet:
		EquippmentSlots.HelmetSlot.InventoryItemObject = ItemObject;
		EquippmentSlots.HelmetSlot.EqippedEffectHandle = ActiveGEHandle;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.HelmetSlot);
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.HelmetSlot);
		return true;
		break;
	case EEquipSlotType::Necklace:
		EquippmentSlots.NecklaceSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.NecklaceSlot);
		EquippmentSlots.NecklaceSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.NecklaceSlot);
		return true;
		break;
	case EEquipSlotType::Shoulder:
		EquippmentSlots.ShoulderSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.ShoulderSlot);
		EquippmentSlots.ShoulderSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.ShoulderSlot);
		return true;
		break;
	case EEquipSlotType::Chest:
		EquippmentSlots.ChestSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.ChestSlot);
		EquippmentSlots.ChestSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.ChestSlot);
		return true;
		break;
	case EEquipSlotType::Back:
		EquippmentSlots.BackSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BackSlot);
		EquippmentSlots.BackSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BackSlot);
		return true;
		break;
	case EEquipSlotType::Gloves:
		EquippmentSlots.GlovesSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.GlovesSlot);
		EquippmentSlots.GlovesSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.GlovesSlot);
		return true;
		break;
	case EEquipSlotType::Belt:
		EquippmentSlots.BeltSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BeltSlot);
		EquippmentSlots.BeltSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BeltSlot);
		return true;
		break;
	case EEquipSlotType::Legs:
		EquippmentSlots.LegsSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.LegsSlot);
		EquippmentSlots.LegsSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.LegsSlot);
		return true;
		break;
	case EEquipSlotType::Boots:
		EquippmentSlots.BootsSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BootsSlot);
		EquippmentSlots.BootsSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BootsSlot);
		return true;
		break;
	case EEquipSlotType::RingRight:
		EquippmentSlots.RingRightSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.RingRightSlot);
		EquippmentSlots.RingRightSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.RingRightSlot);
		return true;
		break;
	case EEquipSlotType::RingLeft:
		EquippmentSlots.RingLeftSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.RingLeftSlot);
		EquippmentSlots.RingLeftSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.RingLeftSlot);		
		return true;
		break;
	case EEquipSlotType::Bag:
		EquippmentSlots.BagSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.BagSlot);
		EquippmentSlots.BagSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.BagSlot);
		return true;
		break;
	case EEquipSlotType::WeaponLeft:
		EquippmentSlots.WeaponLeftSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.WeaponLeftSlot);
		EquippmentSlots.WeaponLeftSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.WeaponLeftSlot);
		return true;
		break;
	case EEquipSlotType::WeaponRight:
		EquippmentSlots.WeaponRightSlot.InventoryItemObject = ItemObject;
		OnEquippmentSlotUpdated.Broadcast(EquippmentSlots.WeaponRightSlot);
		EquippmentSlots.WeaponRightSlot.EqippedEffectHandle = ActiveGEHandle;
		ClientBroadcastEquippmentUpdateDelegate(EquippmentSlots.WeaponRightSlot);
		return true;
		break;

	default:
		print(FString("Set Equip slot: default case, return false"));
		return false;
	}

	print(FString("Faield to Equip item due to invalid EquipSlotType"));
	return false;
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
