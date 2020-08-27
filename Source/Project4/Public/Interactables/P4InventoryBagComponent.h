// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "Interactables/ItemArmorDataAsset.h"
#include "Interactables/ItemWeaponDataAsset.h"
#include "P4InventoryBagComponent.generated.h"


/**
 * Lots of data structures here but theyre for good reason as items in inventory should really just be data
 * not some hidden actor we force into our 'bags'. 
 */

/**
 * Nullptr counts as empty slot
 */
USTRUCT(BlueprintType)
struct FInventoryItemStruct
{
	GENERATED_USTRUCT_BODY()
	
	/* true if this is just empty inventory slot (should not be spawned actor-item) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		bool bIsEmpty;

	/* Stack Count for this item in inventory pouch */
	/* IT IS ESSENTIAL THAT THIS is the actual stack count over value in itembasedata asset (data assets should never be written into on play) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int StackCount;

	/* Stack Count for this item drop/stack */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		UItemBaseDataAsset* ItemBaseDataAsset;

	FInventoryItemStruct()
	{
		bIsEmpty = true;
		StackCount = -1;
		ItemBaseDataAsset = nullptr;
	}

	FInventoryItemStruct(UItemBaseDataAsset* InputItemBaseAsset, int StackCount)
	{
		bIsEmpty = false;
		StackCount = StackCount;
		ItemBaseDataAsset = InputItemBaseAsset;
	}

	FInventoryItemStruct(FInventoryItemStruct* InputInventoryItemStruct)
	{
		bIsEmpty = false;
		StackCount = InputInventoryItemStruct->StackCount;
		ItemBaseDataAsset = InputInventoryItemStruct->ItemBaseDataAsset;
		ItemBaseDataAsset->ItemInfo = FItemBaseInfoStruct(InputInventoryItemStruct->ItemBaseDataAsset->ItemInfo);
	}
};

/* each enum represents a slot in ui character equip slots */
UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),  // "None" for empty slots (this shouldnt be necessary at this depth)
	Helmet			UMETA(DispalyName = "Helmet"),
	Necklace		UMETA(DispalyName = "Necklace"),
	Shoulder		UMETA(DispalyName = "Shoulder"),
	Chest			UMETA(DispalyName = "Chest"),
	Back			UMETA(DispalyName = "Back"),
	Gloves			UMETA(DispalyName = "Gloves"),
	Belt			UMETA(DispalyName = "Belt"),
	Legs			UMETA(DispalyName = "Legs"),
	Boots			UMETA(DispalyName = "Boots"),
	RingLeft		UMETA(DispalyName = "RingLeft"),
	RingRight		UMETA(DispalyName = "RingRight"),
	Bag				UMETA(DispalyName = "Bag"),
	WeaponLeft		UMETA(DisplayName = "WeaponLeft"),
	WeaponRight		UMETA(DisplayName = "WeaponRight")
};

USTRUCT(BlueprintType)
struct FEquippmentSlotStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FInventoryItemStruct InventoryItemStruct;

	// Pretty sure storing the enum is unecessary with get/set functions doing the work for us 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		EEquipSlotType EquipSlotType;

	/* used for quick access to remove attribute bonuses on unequip */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FActiveGameplayEffectHandle EqippedEffectHandle;

	FEquippmentSlotStruct()
	{

		InventoryItemStruct = FInventoryItemStruct();
		EquipSlotType = EEquipSlotType::None;
		EqippedEffectHandle = FActiveGameplayEffectHandle();
	}


	FEquippmentSlotStruct(EEquipSlotType EquipType)
	{
		InventoryItemStruct = FInventoryItemStruct();
		EquipSlotType = EquipType;
		EqippedEffectHandle = FActiveGameplayEffectHandle();
	}
};


USTRUCT(BlueprintType)
struct FEquippedItemsStruct
{
	GENERATED_USTRUCT_BODY()

	/* Armor stuff */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct HelmetSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct NecklaceSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct ShoulderSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct ChestSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct BackSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct GlovesSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct BeltSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct LegsSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct BootsSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct RingRightSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct RingLeftSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct BagSlot;

	/* Weapon Stuff */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct WeaponLeftSlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FEquippmentSlotStruct WeaponRightSlot;

	// TODO: add weapon type enum (left/right/both) make sure if weapon enum exists then set armor to none vice versa
	FEquippedItemsStruct()
	{
		HelmetSlot = FEquippmentSlotStruct(EEquipSlotType::Helmet);
		NecklaceSlot = FEquippmentSlotStruct(EEquipSlotType::Necklace);
		ShoulderSlot = FEquippmentSlotStruct(EEquipSlotType::Shoulder);
		ChestSlot = FEquippmentSlotStruct(EEquipSlotType::Chest);
		BackSlot = FEquippmentSlotStruct(EEquipSlotType::Back);
		GlovesSlot = FEquippmentSlotStruct(EEquipSlotType::Gloves);
		BeltSlot = FEquippmentSlotStruct(EEquipSlotType::Belt);
		LegsSlot = FEquippmentSlotStruct(EEquipSlotType::Legs);
		BootsSlot = FEquippmentSlotStruct(EEquipSlotType::Boots);
		RingRightSlot = FEquippmentSlotStruct(EEquipSlotType::RingLeft);
		RingLeftSlot = FEquippmentSlotStruct(EEquipSlotType::RingRight);
		BagSlot = FEquippmentSlotStruct(EEquipSlotType::Bag);
		WeaponLeftSlot = FEquippmentSlotStruct(EEquipSlotType::WeaponLeft);
		WeaponRightSlot = FEquippmentSlotStruct(EEquipSlotType::WeaponRight);
	}
};


/* On item added to inventory */
/* Dynamic due to custom struct*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotUpdated, const FInventoryItemStruct&, NewitemInfo, int, Index);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippmentSlotUpdated, const FEquippmentSlotStruct&, NewEquippmentInfo);


/**
 * Inventory class, this contains array of item structures to obviously hold info about the items the player holds (pepega).
 *  - This class also holds an array of currently equipped items. This part could be ported to its own component if needed but kept together due to their somewhat similar needs
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT4_API UP4InventoryBagComponent : public UActorComponent
{
	GENERATED_BODY()

	/* On item removed from inventory */
	//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, UItemBaseDataAsset* /* NewItemInfo */, int /* InventoryIndex */);

public:	
	// Sets default values for this component's properties
	UP4InventoryBagComponent(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* in Kg */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float MaxWeight = 100.f;

	/* number of slots or amount of unique items to store in bag */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int NumInventorySlots = 20;
	
	/* main currency count */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int MainCurrencyCount = 0;

	// Helper for placing items in inventory
	void FindFirstEmptySpot(bool& WasSuccessful, int& Index);

	/*******************/
	/*     Utility     */
	/*******************/
	// Utility codes are bulky and so are shoved to the bottom of .cpp
	UFUNCTION(Category = "Utility")
		EEquipSlotType ArmorTypeToEquipSlotType(EArmorType ArmorType, bool IsRightFinger);

	// isntead of for looping get reference to equipslot with hardcoded indexes (REQUIRES MANUAL MAINTENANCE FOR NEW/CHANGED SLOTS)
	UFUNCTION(Category = "Utility")
		bool GetEquipSlotInfo(EEquipSlotType EquipSlotType, FEquippmentSlotStruct& ReturnSlot);
	
	// Similar structure to above but sets its info with inventoryItemStruct
	UFUNCTION(Category = "Utility")
		bool SetEquipSlotInfo(EEquipSlotType EquipSlotType, FInventoryItemStruct ItemStruct, FActiveGameplayEffectHandle ActiveGEHandle);

public:	

	/* Items stored in Row-major order (topleft is lowest, bottom right is highest) */
	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere)
		TArray<FInventoryItemStruct> InventoryArray;

	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere)
		FEquippedItemsStruct EquippmentSlots;

	/*******************/
	/*    Delegates    */
	/*******************/

	UPROPERTY(BlueprintAssignable, Category = "Delegates | Inventory")
		FOnInventorySlotUpdated OnInventorySlotUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Delegates | Equippment")
		FOnEquippmentSlotUpdated OnEquippmentSlotUpdated;

	//UPROPERTY(BlueprintAssignable)
	//	FOnItemRemoved ItemRemovedDelegate;

	// TODO: make function to read players inventory and equips
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void InitializeEmptyInventory();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void InitializeEmptyEquippment();

	/* returns inventory index(es) of items with same itemID in ItemBaseDataAsset */
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void FindItemsFromItemData(const FInventoryItemStruct& SearchItem, bool& WasFound, TArray<int>& FoundIndexes);

	/*********************/
	/* Server/Client RPCs*/
	/*********************/

	/* From server broadcast update inventory delegate to client */
	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "Utility")
		void ClientBroadcastInventoryUpdateDelegate(const FInventoryItemStruct& NewItemInfo, int Index);
	void ClientBroadcastInventoryUpdateDelegate_Implementation(const FInventoryItemStruct& NewItemInfo, int Index);
	bool ClientBroadcastInventoryUpdateDelegate_Validate(const FInventoryItemStruct& NewItemInfo, int Index) { return true; }

	/* From server broadcast update equippment delegate to client */
	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "Utility")
		void ClientBroadcastEquippmentUpdateDelegate(const FEquippmentSlotStruct& NewEquippmentInfo);
	void ClientBroadcastEquippmentUpdateDelegate_Implementation(const FEquippmentSlotStruct& NewEquippmentInfo);
	bool ClientBroadcastEquippmentUpdateDelegate_Validate(const FEquippmentSlotStruct& NewEquippmentInfo) { return true; }


	/* Return true if item added to array with index to placed item, else return false (e.g inventory full) */
	/* This should push the item to the first available slot */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Utility")
		void ServerAddItemToInventory(const FInventoryItemStruct& NewItem, AActor* Instigator);
	void ServerAddItemToInventory_Implementation(const FInventoryItemStruct& NewItem, AActor* Instigator);
	bool ServerAddItemToInventory_Validate(const FInventoryItemStruct& NewItem, AActor* Instigator) { return true; }

			/***************************/
			/*  Server/Client RPCs     */
			/*  - User input Callbacks */
			/***************************/

	/* client request drop item (from inventory widget)  */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Utility")
		void ServerDropItemFromInventory(int InventoryIndex);
	void ServerDropItemFromInventory_Implementation(int InventoryIndex);
	bool ServerDropItemFromInventory_Validate(int InventoryIndex) { return true; }


	/**** EQUIPPING ITEMS FROM INVENTORY ****/

	/* client request equip item (from inventory widget)  */
	/* bool Used for weapons and rings for which side user inputted (right or left) */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Utility")
		void ServerEquipItemFromInventory(int InventoryIndex, bool IsRightSide);
	void ServerEquipItemFromInventory_Implementation(int InventoryIndex, bool IsRightSide);
	bool ServerEquipItemFromInventory_Validate(int InventoryIndex, bool IsRightSide) { return true; }

	/* Server helper function for doing ArmorType Equip */
	void EquipArmorItemFromInventory(int InventoryIndex, bool IsRightFinger, FInventoryItemStruct& Item);

	/* Server Helper funciton for doing weapontype Equip */
	void EquipWeaponItemFromInventory(int InventoryIndex, bool IsRightHand, FInventoryItemStruct& Item);


	/**** UNEQUIPPING ITEMS FROM INVENTORY ****/

	/* client request Unequip item (from Characterinfo widget), if ring then use bool to specify finger, else ignore  */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Utility")
		void ServerUnEquipItemFromInventory(EEquipSlotType EquipSlotType);
	void ServerUnEquipItemFromInventory_Implementation(EEquipSlotType EquipSlotType);
	bool ServerUnEquipItemFromInventory_Validate(EEquipSlotType EquipSlotType) { return true; }


	/* Overrides */

	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
