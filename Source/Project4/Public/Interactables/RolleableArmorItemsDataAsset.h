// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interactables/ItemArmorDataAsset.h"
#include "Interactables/RolleableAttributesDataAsset.h"
#include "RolleableArmorItemsDataAsset.generated.h"

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


/**
 * 
 */
UCLASS()
class PROJECT4_API URolleableArmorItemsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	URolleableArmorItemsDataAsset();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
		TMap<EArmorType, FRolleableArmorItemsStruct> ArmorTypeToItemAssets;

	UFUNCTION()
		void GetRandomArmorItem(EArmorType ArmorType, TSubclassOf<class AP4MobCharacterBase> MobClass, FRolleableArmorItemStruct& RandomArmorItem);

	UFUNCTION()
		void SumTotalWeights();
private:


	UPROPERTY()
		TMap<EArmorType, float> TotalWeights;
};
