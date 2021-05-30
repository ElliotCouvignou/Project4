// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/SkillModifierDropActor.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "P4PreGameLobbyGameState.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"



// Sets default values
ASkillModifierDropActor::ASkillModifierDropActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

void ASkillModifierDropActor::OnInteract(const AP4PlayerCharacterBase* SourceActor)
{
	if (SourceActor)
	{
		UP4PlayerAbilitySystemComponent* ASC = Cast<UP4PlayerAbilitySystemComponent>(SourceActor->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->Server_OnPlayerSkillModifierDropInteracted();
		}
		APlayerController* PC = SourceActor->GetController<APlayerController>();
		if (PC && !bIsTestActor)
		{
			SeenPlayers.AddUnique(PC);
			AP4PreGameLobbyGameState* GS = Cast<AP4PreGameLobbyGameState>(GetWorld()->GetGameState());
			if (GS && SeenPlayers.Num() == GS->PlayerNumberMap.Num())
			{
				// all palyers interacted so destroy
				Destroy();
			}
		}
	}
}

// Called when the game starts or when spawned
void ASkillModifierDropActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillModifierDropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

