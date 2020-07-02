// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "HealingCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UHealingCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()
	
public:

	/* Actual Damage calculations */
	UFUNCTION()
		virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

};
