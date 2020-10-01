// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/AutoAttackAbility.h"
#include "AbilitySystem/P4AbilityProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


UAutoAttackAbility::UAutoAttackAbility()
{

}

void UAutoAttackAbility::SpawnBowProjectile(const FTransform& Transform, const FGameplayEffectSpecHandle& GameplayEffect, const TSubclassOf<class AP4AbilityProjectile> Class, const float Range, const float InitialSpeed, AP4AbilityProjectile* SpawnedActor)
{
	
	AP4AbilityProjectile* RetSpawn = Cast<AP4AbilityProjectile>(GetWorld()->SpawnActorDeferred<AActor>(Class, Transform, GetOwningActorFromActorInfo(), (APawn*)GetAvatarActorFromActorInfo(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (RetSpawn)
	{
		RetSpawn->Range = Range;
		RetSpawn->ProjectileMovement->InitialSpeed = InitialSpeed;
		RetSpawn->EffectSpecHandle = GameplayEffect;
		RetSpawn->FinishSpawning(Transform);
	}
	SpawnedActor = RetSpawn;
}
