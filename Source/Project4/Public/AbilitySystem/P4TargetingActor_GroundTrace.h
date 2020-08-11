// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "P4TargetingActor_GroundTrace.generated.h"

/**
 *  This class is only useful for projecting the target with the offseted crosshair.
 *  If the crosshair becomes centered then we can default to this parent class
 */
UCLASS(Blueprintable)
class PROJECT4_API AP4TargetingActor_GroundTrace : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_UCLASS_BODY()

protected:

	void AimWithPlayerCrosshair(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd) const;


	virtual FHitResult PerformTrace(AActor* InSourceActor) override;

	
};