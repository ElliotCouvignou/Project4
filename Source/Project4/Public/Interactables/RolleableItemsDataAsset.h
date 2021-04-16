// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interactables/ItemArmorDataAsset.h"
#include "Interactables/ItemWeaponDataAsset.h"
#include "Interactables/RolleableAttributesDataAsset.h"
#include "RolleableItemsDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FRolleableArmorItemStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		UItemArmorDataAsset* ArmorDataAsset;

	/* Weight for this item to drop */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float Weight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TMap<EItemRank, float> ItemRankModifiers;

	/* optional influences to weights and values of generic rolleable attributes */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FRolleableAttributeStruct> RolleableAttributeModifiers;

	/* This item can only be dropped on mobs included in array, empty = everyone */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<TSubclassOf<class AP4MobCharacterBase>> MobsToDropOn;

	/* used for exclude this item from characters, keeping empty does nothing */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<TSubclassOf<class AP4MobCharacterBase>> MobsToNotDropOn;

	FRolleableArmorItemStruct()
	{
		Weight = 1.f;

	}



};

/* info used in inherited classes as BP readable struct on GE attr changes */
USTRUCT(BlueprintType)
struct FRolleableArmorItemsStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FRolleableArmorItemStruct> ArmorItems;


	FRolleableArmorItemsStruct()
	{

	}

};



USTRUCT(BlueprintType)
struct FRolleableWeaponItemStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		UItemWeaponDataAsset* WeaponDataAsset;

	/* Weight for this item to drop */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float Weight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TMap<EItemRank, float> ItemRankModifiers;

	/* optional influences to weights and values of generic rolleable attributes */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FRolleableAttributeStruct> RolleableAttributeModifiers;

	/* This item can only be dropped on mobs included in array, empty = everyone */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<TSubclassOf<class AP4MobCharacterBase>> MobsToDropOn;

	/* used for exclude this item from characters, keeping empty does nothing */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<TSubclassOf<class AP4MobCharacterBase>> MobsToNotDropOn;

	FRolleableWeaponItemStruct()
	{
		Weight = 1.f;

	}



};



/* info used in inherited classes as BP readable struct on GE attr changes */
USTRUCT(BlueprintType)
struct FRolleableWeaponItemsStruct
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FRolleableWeaponItemStruct> WeaponItems;


	FRolleableWeaponItemsStruct()
	{

	}

};



/**
 * 
 */
UCLASS()
class PROJECT4_API URolleableItemsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	URolleableItemsDataAsset();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
		TMap<EArmorType, FRolleableArmorItemsStruct> ArmorTypeToItemAssets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
		TMap<EWeaponType, FRolleableWeaponItemsStruct> WeaponTypeToItemAssets;

	UFUNCTION()
		void GetRandomArmorItem(EArmorType ArmorType, TSubclassOf<class AP4MobCharacterBase> MobClass, FRolleableArmorItemStruct& RandomArmorItem);

	UFUNCTION()
		void GetRandomWeaponItem(EWeaponType WeaponType, TSubclassOf<class AP4MobCharacterBase> MobClass, FRolleableWeaponItemStruct& RandomWeaponItem);

	UFUNCTION()
		void SumTotalWeights();
private:


	UPROPERTY()
		TMap<EArmorType, float> TotalArmorWeights;

	UPROPERTY()
		TMap<EWeaponType, float> TotalWeaponWeights;
};
