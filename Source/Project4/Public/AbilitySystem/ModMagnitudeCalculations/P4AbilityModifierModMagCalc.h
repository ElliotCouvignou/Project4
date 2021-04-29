// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "P4AbilityModifierModMagCalc.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4AbilityModifierModMagCalc : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	

public:
	UP4AbilityModifierModMagCalc();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int AbilityModifierValueIndex = 0;

	UFUNCTION()
		virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
