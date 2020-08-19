// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "P4InventoryBagComponent.generated.h"


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
	/* IT IS ESSENTIAL THAT THIS is the actual stack count over value in itembasedata asset*/
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
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT4_API UP4InventoryBagComponent : public UActorComponent
{
	GENERATED_BODY()

	/* On item added to inventory */
	/* Dynamic due to custom struct*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotUpdated, const FInventoryItemStruct& , NewitemInfo, int , Index);

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

public:	


	/* Items stored in Row-major order (topleft is lowest, bottom right is highest) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FInventoryItemStruct> InventoryArray;


	/*******************/
	/*    Delegates    */
	/*******************/

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnInventorySlotUpdated OnInventorySlotUpdated;

	//UPROPERTY(BlueprintAssignable)
	//	FOnItemRemoved ItemRemovedDelegate;

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void InitializeEmptyInventory();
	
	/* returns inventory index(es) of items with same itemID in ItemBaseDataAsset */
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void FindItemsFromItemData(const FInventoryItemStruct& SearchItem, bool& WasFound, TArray<int>& FoundIndexes);

	/* Return true if item added to array with index to placed item, else return false (e.g inventory full) */
	/* This should push the item to the first available slot */
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void AddItemToInventory(UPARAM(ref) FInventoryItemStruct& NewItem, int& ReturnArrayindex, bool& WasSucessful);

	/* same as above but places the new item in a specific spot. Should check for stacking */
	//UFUNCTION(BlueprintCallable, Category = "Utility")
	//	void AddItemToInventorySpot(const UItemBaseDataAsset* NewItem, const int ArrayIndex, bool& WasSucessful);
	//
	///* Called on an item to be removed (search and then remove), returns true if succesful with index to removed item */
	//UFUNCTION(BlueprintCallable, Category = "Utility")
	//	void RemoveItemFromInventory(const UItemBaseDataAsset* NewItem, int ReturnArrayindex, bool& WasSucessful);
	//
	///* same as above but places the new item in a specific spot. Should check for stacking */
	//UFUNCTION(BlueprintCallable, Category = "Utility")
	//	void RemoveItemFromInventorySpot(UItemBaseDataAsset* RemovedItem, const int ArrayIndex, bool& WasSucessful);



	
};
