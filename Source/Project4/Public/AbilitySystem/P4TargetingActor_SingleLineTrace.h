// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "P4TargetingActor_SingleLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4TargetingActor_SingleLineTrace : public AGameplayAbilityTargetActor_SingleLineTrace
{
	GENERATED_UCLASS_BODY()
	
protected:

	void AimWithPlayerCrosshair(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd) const;

	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
};
