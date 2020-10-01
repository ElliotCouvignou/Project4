// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "AutoAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UAutoAttackAbility : public UP4GameplayAbility
{
	GENERATED_BODY()


public:

	UAutoAttackAbility();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ranged MH | Bow")
		TSubclassOf<class AP4AbilityProjectile> BowProjectileClass;

	UFUNCTION(BlueprintCallable)
		void SpawnBowProjectile(const FTransform& Transform, const FGameplayEffectSpecHandle& GameplayEffect, const TSubclassOf<class AP4AbilityProjectile> Class, const float Range, const float InitialSpeed, AP4AbilityProjectile* SpawnedActor);
};
