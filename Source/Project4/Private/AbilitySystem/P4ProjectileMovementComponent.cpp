// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

void UP4ProjectileMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (HomingTarget && bIsHomingProjectile)
	{
		RandomHomingOffset.X = FMath::RandRange(0.f, HomingTarget->GetCapsuleComponent()->GetScaledCapsuleRadius() * RandomHomingOffsetMagnitude);
		RandomHomingOffset.Y = FMath::RandRange(0.f, HomingTarget->GetCapsuleComponent()->GetScaledCapsuleRadius() * RandomHomingOffsetMagnitude);
		RandomHomingOffset.Z = FMath::RandRange(0.f, HomingTarget->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * RandomHomingOffsetMagnitude);
	}
}

FVector UP4ProjectileMovementComponent::ComputeHomingLocation() const
{
	return HomingTarget->GetActorLocation() + RandomHomingOffset;
}

FVector UP4ProjectileMovementComponent::ComputeAcceleration(const FVector& InVelocity, float DeltaTime) const
{
	FVector Acceleration(FVector::ZeroVector);

	Acceleration.Z += GetGravityZ();

	Acceleration += PendingForceThisUpdate;

	if (bIsHomingProjectile && HomingTarget)
	{
		Acceleration += ComputeHomingAcceleration(InVelocity, DeltaTime);
	}

	return Acceleration;
}


FVector UP4ProjectileMovementComponent::ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const
{
	FVector HomingAcceleration = ((ComputeHomingLocation() - UpdatedComponent->GetComponentLocation()).GetSafeNormal() * HomingAccelerationMagnitude);
	return HomingAcceleration;
}


// ovveride replciation with replication variables
void UP4ProjectileMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(UP4ProjectileMovementComponent, HomingTarget);
}