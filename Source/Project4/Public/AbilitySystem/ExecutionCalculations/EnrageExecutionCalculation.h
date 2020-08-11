// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EnrageExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UEnrageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()
	
public:

	UFUNCTION()
		virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

};
