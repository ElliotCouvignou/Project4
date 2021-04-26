// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "AbilitySystem/P4AbilityModifier.h"
#include "P4AbilityModifierGameplayEffect.generated.h"



/**
 * 
 */
UCLASS()
class PROJECT4_API UP4AbilityModifierGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()


public:
	UP4AbilityModifierGameplayEffect(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		UTexture2D* AbilityModifierIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		EAbilityModifierRank AbilityModifierRank;
	
	/* Name of row to access ability descriptions for UI from description table */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		FName AbilityModifierTooltipDataTableRowName;

	/* Weight to roll this modifier compared to other valid ones (valid queried first) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		float Weight = 1.f;

};
