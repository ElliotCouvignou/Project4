// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4AbilityProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AP4AbilityProjectile::AP4AbilityProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AP4AbilityProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP4AbilityProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

