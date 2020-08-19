// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ItemBaseDataAsset.generated.h"


/**
 *	
 */
USTRUCT(BlueprintType)
struct FItemBaseInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/* Icon to display for inventory widgets */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals")
		UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		bool bIsStackable;

	/* weight for each unique instance of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float ItemWeight;

	/* Max Stack Count for this item drop/stack */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int MaxStackCount;

	/* Item ID for every item type (e.g all 'feathers' have same ItemID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int32 ItemID;

	/* Empty space 'item' data asset */
	FItemBaseInfoStruct()
	{
		ItemIcon = nullptr;
		ItemName = FText::FromString("Empty");
		bIsStackable = false;
		ItemWeight = 0.f;
		ItemID = -1;
	}
};


/**
 * 
 */
UCLASS()
class PROJECT4_API UItemBaseDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AbilitySet)
		FItemBaseInfoStruct	ItemInfo;

	/* Attribute Set Data Table for Attribute Bonuses
		Can Be left empty */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attributes)
		UDataTable* AttrDataTable;

	
};
