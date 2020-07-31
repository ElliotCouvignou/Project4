// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "LacerationExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API ULacerationExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RefundDistance = 785.5f;  // 15m * 52.5

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ManaRefundAmount = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CooldownRefundAmount = 5.f;  // in seconds

	UFUNCTION()
		virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

};
	

