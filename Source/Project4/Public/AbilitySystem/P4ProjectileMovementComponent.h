// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "P4ProjectileMovementComponent.generated.h"

class ACharacter;

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4ProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	

	virtual void InitializeComponent() override;

public:

	/* 1 = location can be anywhere in capsule, 0 = right at center*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Project4 | Homing")
		float RandomHomingOffsetMagnitude = 0.8f;

	// right now only this is replicated
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Project4 | Homing")
		ACharacter* HomingTarget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Project4 | Homing")
		FVector RandomHomingOffset = FVector();


	FVector ComputeHomingLocation() const;

	/** Compute the acceleration that will be applied
		Same as base but changes to check for homing */
	virtual FVector ComputeAcceleration(const FVector& InVelocity, float DeltaTime) const;

	/** Allow the projectile to track towards its homing target. */
	virtual FVector ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const;
};
