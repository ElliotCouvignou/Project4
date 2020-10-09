// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "AbilitySystem/AttributeSets/P4BaseAttributeSet.h"
#include "ItemBaseDataAsset.generated.h"


// Holds the types of items e.g. Armor, weapons, consumable, material, etc.
// THIS DOESNT HOLD INFO ABOUT RARITY (e.g. white,green,blue...), thats in another enum
UENUM(BlueprintType)
enum class EItemType : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),  // "None" for empty slots
	Weapon			UMETA(DisplayName = "Weapon"),
	Armor			UMETA(DisplayName = "Armor"),
	Consumable		UMETA(DisplayName = "Consumable"),
	Material		UMETA(DisplayName = "Material"),
	Currency		UMETA(DisplayName = "Currency")
};


// THIS HOLDs INFO ABOUT RARITY 
UENUM(BlueprintType)
enum class EItemRank : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),		// "None" for empty slots
	Common			UMETA(DisplayName = "Common"),		//  White
	Uncommon		UMETA(DisplayName = "Uncommon"),	//  Green
	Rare			UMETA(DisplayName = "Rare"),		//  Blue
	Legendary		UMETA(DisplayName = "Legendary")    //  Orange/Yellow
};

/**
 *	 TODO: Optimize memory space 
 *   ATM this takes around 45-60 bytes (depending on name length)
 */
USTRUCT(BlueprintType)
struct FItemBaseInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/* specific class to spawn if this item needs to be spawned again */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		UClass* ItemClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		EItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		EItemRank ItemRank;

	/* Static mesh to represent the actor item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		UStaticMesh* ItemStaticMesh;

	/* Icon to display for inventory widgets */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FString ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		bool bIsStackable;

	/* weight for each unique instance of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float ItemWeight;

	/* Max Stack Count for this item drop/stack */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int MaxStackCount;

	/* Level requirement to use/wear item (can be left empty if irrelevant) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int LevelRequirement;

	/* Item ID for every item type (e.g all 'feathers' have same ItemID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		int32 ItemID;

	/* Empty space 'item' data asset */
	FItemBaseInfoStruct()
	{
		ItemClass = nullptr;
		ItemType = EItemType::None;
		ItemRank = EItemRank::None;
		ItemStaticMesh = nullptr;
		ItemIcon = nullptr;
		ItemName = FString("Empty");
		bIsStackable = false;
		ItemWeight = 0.f;
		MaxStackCount = 0.f;
		LevelRequirement = 0;
		ItemID = -1;
	}

	// TODO: set constructor (copy) version 
	FItemBaseInfoStruct(FItemBaseInfoStruct& Other)
	{
		ItemClass = Other.ItemClass;
		ItemType = Other.ItemType;
		ItemRank = Other.ItemRank;
		ItemStaticMesh = Other.ItemStaticMesh;
		ItemIcon = Other.ItemIcon;
		ItemName = Other.ItemName;
		bIsStackable = Other.bIsStackable;
		ItemWeight = Other.ItemWeight;
		MaxStackCount = Other.MaxStackCount;
		LevelRequirement = Other.LevelRequirement;
		ItemID = Other.ItemID;
	}
};

/* info used in inherited classes as BP readable struct on GE attr changes */
USTRUCT(BlueprintType)
struct FAttributeDataUIStruct
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FString AttributeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float AttributeValue;

	FAttributeDataUIStruct()
	{
		AttributeName = FString();
		AttributeValue = 0.f;
	}

	FAttributeDataUIStruct(FString Name, float Value)
	{
		AttributeName = Name;
		AttributeValue = Value;
	}
};


/**
 * Base level of items in data, do not use this as an item ONLY use the subclasses of this
 */
UCLASS(BlueprintType)
class PROJECT4_API UItemBaseDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UItemBaseDataAsset();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
		FItemBaseInfoStruct	ItemInfo;

};
