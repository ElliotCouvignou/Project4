// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "P4ItemBaseObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECT4_API UP4ItemBaseObject : public UObject
{
	GENERATED_BODY()

public:

	UP4ItemBaseObject();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		EItemType ItemType;

	/* true if this is just empty inventory slot (should not be spawned actor-item) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		bool bIsEmpty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		bool bIsStackable;

	/* Stack Count for this item in inventory pouch */
	/* IT IS ESSENTIAL THAT THIS is the actual stack count over value in itembasedata asset (data assets should never be written into on play) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int StackCount;

	/* Max Stack Count for this item drop/stack */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int MaxStackCount;

	/* specific class to spawn if this item needs to be spawned again */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		UClass* DroppedItemClass;

	/* Icon to display for inventory widgets */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FString ItemName;

	/* weight for each unique instance of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float ItemWeight;

	/* Item ID for every item type (e.g all 'feathers' have same ItemID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int32 ItemID;

	/* Static mesh to represent the actor item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Optional")
		UStaticMesh* DroppedItemStaticMesh;

	/* Holds Active GE for pickup (Adds weight to carryweight player-attribute) */
	UPROPERTY(BlueprintReadOnly, Category = "Generated")
		FActiveGameplayEffectHandle ActiveGE;


	UFUNCTION(BlueprintCallable)
		virtual void InitializeFromDataAsset(UItemBaseDataAsset* Data);
};
