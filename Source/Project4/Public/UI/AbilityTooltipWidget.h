// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "AbilityTooltipWidget.generated.h"




UENUM(BlueprintType)
enum class EAbilityTooltipCostType : uint8
{
	Mana				UMETA(DisplayName = "Mana"),
	Health				UMETA(DisplayName = "Health"),
	Stamina				UMETA(DisplayName = "Stamina"),
	Other				UMETA(DisplayName = "Other"),  // used for some charge system or idk
};

UENUM(BlueprintType)
enum class EAbilityTooltipCostValueType : uint8
{
	Flat				UMETA(DisplayName = "Flat"),
	PercentMax			UMETA(DisplayName = "PercentMax"),
	PercentCurrent		UMETA(DisplayName = "PercentCurrent"),
};

UENUM(BlueprintType)
enum class ETooltipValueType : uint8
{
	None				UMETA(DisplayName = "None"),    // no changes
	Percent				UMETA(DisplayName = "Percent"), // decimal converted to %
	Meters				UMETA(DisplayName = "Meters")  // Meters (*52.5)
};


/* Ability tooltip displayed to player is generated with this struct info*/
USTRUCT(BlueprintType)
struct FAbilityTooltipCost
{
	GENERATED_USTRUCT_BODY()

	/* If false, no tooltip will be generated when hovering this ability */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cost | Type")
		EAbilityTooltipCostType CostType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cost | Type")
		EAbilityTooltipCostValueType CostValueType;

	/* Curve of cost to player-level */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cost | Curve")
		UCurveTable* CostCurve;

	/* flat cost if curve isn't necessary (if a curve table exists this var is ignored) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cost | Flat ")
		float FlatCost;

};

/* Ability tooltip displayed to player is generated with this struct info*/
USTRUCT(BlueprintType)
struct FAbilityTooltipStruct
{
	GENERATED_USTRUCT_BODY()

	/* If false, no tooltip will be generated when hovering this ability */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Essential")
		bool bGenerateTooltip;

	/* Name of ability displayed to players */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Ability Name")
		FText AbilityName;

	/* List of costs */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Costs")
		TArray<FAbilityTooltipCost> Costs;

	/* Curve to scale with ability level */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Cooldown")
		UCurveTable* CooldownCurve;

	/* Curve to scale with ability level */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Cooldown")
		float FlatCooldown;

	/* This should be a rich text style description for those pretty colors */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Descriptions")
		FText Description;

	/* This should where the scaling numbers are added and accesed in Description Ftext */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Descriptions")
		TArray<UCurveTable*> DescriptionNumbers;
};

/* Row structure for data tables on ability descriptions */
/* we use data tables so we dont need to store redundant info in ability class which are recreated on each cast */
USTRUCT(BlueprintType)
struct FP4AbilityDetailsTableRow : public FTableRowBase
{
	GENERATED_BODY()


	/** Details on tooltip and varaibles to help generate UI **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
		FAbilityTooltipStruct TooltipStruct;
};


/* Ability tooltip displayed to player is generated with this struct info*/
USTRUCT(BlueprintType)
struct FAbilityModifierTooltipStruct
{
	GENERATED_USTRUCT_BODY()

	/* If false, no tooltip will be generated when hovering this ability */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Essential")
		bool bGenerateTooltip;

	/* This should be a rich text style description for those pretty colors */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Descriptions")
		FText Description;

	// Used to modify values for UI (e.g Percents and distances modify values shown)
	// Index in array is index to calculation array that this refers to
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tooltip | Values")
		TArray<ETooltipValueType> DescriptionValueTypes;

};

USTRUCT(BlueprintType)
struct FP4AbilityModifierDetailsTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


	/** Details on tooltip and varaibles to help generate UI **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
		FAbilityModifierTooltipStruct TooltipStruct;
};

/**
 * 
 */
UCLASS()
class PROJECT4_API UAbilityTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
};
