// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "Interactables/ItemArmorDataAsset.h"
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


USTRUCT(BlueprintType)
struct FEquippmentSlotStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FInventoryItemStruct InventoryItemStruct;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		EArmorType SlotArmorType;

	/* if EArmorType::Ringr then this variable matters */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		bool bIsRightFinger;

	// TODO: add weapon type enum (left/right/both) make sure if weapon enum exists then set armor to none vice versa

	FEquippmentSlotStruct()
	{
		bIsRightFinger = false;
		InventoryItemStruct = FInventoryItemStruct();
		SlotArmorType = EArmorType::None;
	}

	FEquippmentSlotStruct(EArmorType ArmorType)
	{
		bIsRightFinger = false;
		InventoryItemStruct = FInventoryItemStruct();
		SlotArmorType = ArmorType;
	}

	FEquippmentSlotStruct(EArmorType ArmorType, bool RightFinger)
	{
		InventoryItemStruct = FInventoryItemStruct();
		SlotArmorType = ArmorType;
		bIsRightFinger = RightFinger;
	}
};


USTRUCT(BlueprintType)
struct FEquippedItemsStruct
{
	GENERATED_USTRUCT_BODY()

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

	// TODO: add weapon type enum (left/right/both) make sure if weapon enum exists then set armor to none vice versa
	FEquippedItemsStruct()
	{
		HelmetSlot = FEquippmentSlotStruct(EArmorType::Helmet);
		NecklaceSlot = FEquippmentSlotStruct(EArmorType::Necklace);
		ShoulderSlot = FEquippmentSlotStruct(EArmorType::Shoulder);
		ChestSlot = FEquippmentSlotStruct(EArmorType::Chest);
		BackSlot = FEquippmentSlotStruct(EArmorType::Back);
		GlovesSlot = FEquippmentSlotStruct(EArmorType::Gloves);
		BeltSlot = FEquippmentSlotStruct(EArmorType::Belt);
		LegsSlot = FEquippmentSlotStruct(EArmorType::Legs);
		BootsSlot = FEquippmentSlotStruct(EArmorType::Boots);
		RingRightSlot = FEquippmentSlotStruct(EArmorType::Ring, true);
		RingLeftSlot = FEquippmentSlotStruct(EArmorType::Ring, false);
		BagSlot = FEquippmentSlotStruct(EArmorType::Bag);
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

	// isntead of for looping get reference to equipslot with hardcoded indexes (REQUIRES MANUAL MAINTENANCE FOR NEW/CHANGED SLOTS)
	UFUNCTION(Category = "Utility")
		bool GetEquipSlotInfo(EArmorType ArmorType, bool IsRightFinger, FEquippmentSlotStruct& ReturnSlot);
	
	// Similar structure to above but sets its info with inventoryItemStruct
	UFUNCTION(Category = "Utility")
		bool SetEquipSlotInfo(EArmorType ArmorType, bool IsRightFinger, FInventoryItemStruct ItemStruct);

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

	/* client request equip item (from inventory widget)  */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Utility")
		void ServerEquipItemFromInventory(int InventoryIndex, bool IsRightFinger);
	void ServerEquipItemFromInventory_Implementation(int InventoryIndex, bool IsRightFinger);
	bool ServerEquipItemFromInventory_Validate(int InventoryIndex, bool IsRightFinger) { return true; }

	/* client request Unequip item (from Characterinfo widget), if ring then use bool to specify finger, else ignore  */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Utility")
		void ServerUnEquipItemFromInventory(EArmorType ArmorType, bool IsRightFinger);
	void ServerUnEquipItemFromInventory_Implementation(EArmorType ArmorType, bool IsRightFinger);
	bool ServerUnEquipItemFromInventory_Validate(EArmorType ArmorType, bool IsRightFinger) { return true; }


	/* Overrides */

	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
