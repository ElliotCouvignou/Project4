// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interactables/ItemArmorDataAsset.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "GameplayEffect.h"
#include "Templates/SharedPointer.h"
#include "RolleableAttributesDataAsset.generated.h"

/* info used in inherited classes as BP readable struct on GE attr changes */
USTRUCT(BlueprintType)
struct FRolleableAttributeStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		FGameplayAttribute Attribute;

		/* weight for having this attribute rolled */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float Weight;

	/* Value of this attribute per point of rolling budget */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float Value;

	/* if this attribute is selected, remove the following attributes to be rolled (mutually exclusive) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FGameplayAttribute> AttributesToRemove;

	FRolleableAttributeStruct()
	{
		Weight = 1.f;
		Value = 1.f;
	}

};

/* info used in inherited classes as BP readable struct on GE attr changes */
USTRUCT(BlueprintType)
struct FRolleableAttributeStructArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FRolleableAttributeStruct> Attributes;


	FRolleableAttributeStructArray()
	{

	}

};


/**
 * 
 */
UCLASS(BlueprintType)
class PROJECT4_API URolleableAttributesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
		URolleableAttributesDataAsset();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
		TMap<EArmorType, FRolleableAttributeStructArray> ArmorTypeToRolleableAttributes;

	UFUNCTION()
		void GetRandomAttribute(EArmorType ArmorType, FRolleableAttributeStruct& RolleableAttStruct);

	UFUNCTION()
		void SumTotalWeights();
		
protected:



	UPROPERTY()
		TMap<EArmorType, float> TotalWeight;
};
