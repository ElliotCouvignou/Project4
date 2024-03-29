// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4AbilityAreaHitbox.h"

#include "GameplayEffectTypes.h"
#include "Characters/Project4Character.h"

// Sets default values
AP4AbilityAreaHitbox::AP4AbilityAreaHitbox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}



// Called when the game starts or when spawned
void AP4AbilityAreaHitbox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP4AbilityAreaHitbox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AP4AbilityAreaHitbox::ExecuteHitBoxWithGameplayEffect(UPrimitiveComponent* HitboxComponent, const FGameplayEffectSpecHandle& GameplayEffect, TArray<class AProject4Character*>& HitCharacters)
{
	if (GameplayEffect.IsValid()) {
		TSet<AActor*> OutOverlappingActors;
	
		HitboxComponent->GetOverlappingActors(OutOverlappingActors, TSubclassOf<AProject4Character>());
	
		for (AActor* OtherActor : OutOverlappingActors)
		{
			AProject4Character* PChar = Cast<AProject4Character>(OtherActor);
			if (PChar && OtherActor != GetOwner())
			{
				UAbilitySystemComponent* ASC = PChar->GetAbilitySystemComponent();
				
				if (ASC && PChar->IsAlive())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*GameplayEffect.Data.Get());
					HitCharacters.Add(PChar);
				}
			}
		}
	}
}

void AP4AbilityAreaHitbox::ExecuteHitBoxWithGameplayEffectArray(UPrimitiveComponent* HitboxComponent, const TArray<FGameplayEffectSpecHandle> GameplayEffects, TArray<class AProject4Character*>& HitCharacters)
{
	
	TSet<AActor*> OutOverlappingActors;
	HitboxComponent->GetOverlappingActors(OutOverlappingActors, TSubclassOf<AProject4Character>());
	
	for (AActor* OtherActor : OutOverlappingActors)
	{
		AProject4Character* PChar = Cast<AProject4Character>(OtherActor);
		if (PChar && OtherActor != GetOwner())
		{
			UAbilitySystemComponent* ASC = PChar->GetAbilitySystemComponent();
	
			if (ASC && PChar->IsAlive())
			{
				// apply GE's from array
				for (const FGameplayEffectSpecHandle& GameplayEffectSpec : GameplayEffects)
				{
					ASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data.Get());
					HitCharacters.Add(PChar);
				}
				
			}
		}
	}
	
}

