// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "P4AbilityAreaHitbox.generated.h"

UCLASS()
class PROJECT4_API AP4AbilityAreaHitbox : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AP4AbilityAreaHitbox();

	// expose on spawn sets as parameter for actor spawns of this class
	/* You can still use bp made varaibles for multiple effects, just to more calls */
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	// sends DamageEffectSpecHandle to all overlapping actors in hitbox mesh component
	// Only applies damage to AProject4Character derived classes

	/* Only applies damage */
	UFUNCTION(BlueprintCallable)
		void ExecuteHitBoxWithGameplayEffect(UPrimitiveComponent* HitboxComponent, const FGameplayEffectSpecHandle& GameplayEffect, TArray<class AProject4Character*>& HitCharacters);

	UFUNCTION(BlueprintCallable)
		void ExecuteHitBoxWithGameplayEffectArray(UPrimitiveComponent* HitboxComponent, const TArray<FGameplayEffectSpecHandle> GameplayEffects, TArray<class AProject4Character*>& HitCharacters);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
