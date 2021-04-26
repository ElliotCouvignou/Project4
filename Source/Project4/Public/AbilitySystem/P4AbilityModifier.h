// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P4AbilityModifier.generated.h"

UENUM(BlueprintType)
enum class EAbilityModifierRank : uint8
{
	Common				UMETA(DisplayName = "Common"),
	Rare				UMETA(DisplayName = "Rare"),
	Epic				UMETA(DisplayName = "Epic")
};


/**
 *
 *  This can move somewhere else
 *
 */
USTRUCT(BlueprintType)
struct FP4AbilityModifierBaseInfoStruct
{
	GENERATED_USTRUCT_BODY()

		/* Tag granted to player to gain this modifier (stackeable if allowed) */
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essentials")
		FGameplayTag GrantedTag;

	/* Name of this modifier that player see */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essentials")
		FString Displayname;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		UTexture2D* AbilityModifierIcon;

	/* Name of row to access ability descriptions for UI from description table */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		FName AbilityModifierTooltipDataTableRowName;

	/* Weight to roll this modifier compared to other valid ones (valid queried first) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		float Weight = 1.f;

	FP4AbilityModifierBaseInfoStruct()
	{

	}

	FP4AbilityModifierBaseInfoStruct(const FP4AbilityModifierBaseInfoStruct& other)
	{
		GrantedTag = other.GrantedTag;
		Displayname = other.Displayname;
		AbilityModifierIcon = other.AbilityModifierIcon;
		AbilityModifierTooltipDataTableRowName = other.AbilityModifierTooltipDataTableRowName;
		Weight = other.Weight;
	}
};

/**
 *
 *  This can move somewhere else
 *
 */
USTRUCT(BlueprintType)
struct FP4AbilityModifierInfoStruct : public FP4AbilityModifierBaseInfoStruct
{
	GENERATED_USTRUCT_BODY()



	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		EAbilityModifierRank AbilityModifierRank;

	/* Magnitude of this modifier (since magnitude are procedurally generated) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		float ModifierMagnitude = 0.f;

	FP4AbilityModifierInfoStruct()
	{

	}

	FP4AbilityModifierInfoStruct(const FP4AbilityModifierBaseInfoStruct& other)
	{
		GrantedTag = other.GrantedTag;
		Displayname = other.Displayname;
		AbilityModifierIcon = other.AbilityModifierIcon;
		AbilityModifierTooltipDataTableRowName = other.AbilityModifierTooltipDataTableRowName;
		Weight = other.Weight;
	}

};


USTRUCT(BlueprintType)
struct FP4AbilityModifierInfoMapStruct
{
	GENERATED_USTRUCT_BODY()

	/* Tag granted to player to gain this modifier (stackeable if allowed) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essentials")
		TMap<FGameplayTag, FP4AbilityModifierInfoStruct> AbilityModToInfoMap;
};

USTRUCT(BlueprintType)
struct FP4AbilityModifierBaseInfoMapStruct
{
	GENERATED_USTRUCT_BODY()

		/* Tag granted to player to gain this modifier (stackeable if allowed) */
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essentials")
		TMap<FGameplayTag, FP4AbilityModifierBaseInfoStruct> AbilityModToBaseInfoMap;
};
