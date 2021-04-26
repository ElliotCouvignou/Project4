// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/SkillDropActor.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

// Sets default values
ASkillDropActor::ASkillDropActor() 
	: Super()
{
	bReplicates = false;   // Each player gets their own drops so don't replicate
}

// Called when the game starts or when spawned
void ASkillDropActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkillDropActor::OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result)
{
	if (SourceActor)
	{
		UP4PlayerAbilitySystemComponent* ASC = Cast<UP4PlayerAbilitySystemComponent>(SourceActor->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->Server_OnPlayerSkillDropInteracted();
		}
	}
}

// Called every frame
void ASkillDropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

