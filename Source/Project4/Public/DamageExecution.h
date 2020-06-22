// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "DamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()

public:

	/* Actual Damage calculations */
	UFUNCTION()
		virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

};


