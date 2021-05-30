// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/SkillDropActor.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "P4PreGameLobbyGameState.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

// Sets default values
ASkillDropActor::ASkillDropActor() 
	: Super()
{
	bReplicates = true;   // Each player gets their own drops so don't replicate
}

// Called when the game starts or when spawned
void ASkillDropActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkillDropActor::OnInteract(const AP4PlayerCharacterBase* SourceActor)
{
	if (SourceActor)
	{
		UP4PlayerAbilitySystemComponent* ASC = Cast<UP4PlayerAbilitySystemComponent>(SourceActor->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->Server_OnPlayerSkillDropInteracted();
		}
		APlayerController* PC = SourceActor->GetController<APlayerController>();
		if (PC && !bIsTestActor)
		{
			SeenPlayers.AddUnique(PC);
			AP4PreGameLobbyGameState* GS = Cast<AP4PreGameLobbyGameState>(GetWorld()->GetGameState());
			print(FString("OnInteract Counts: Current: " + FString::FromInt(SeenPlayers.Num()) + "  #total: " + FString::FromInt(GS->PlayerNumberMap.Num())));
			if (GS && SeenPlayers.Num() == GS->PlayerNumberMap.Num())
			{
				// all palyers interacted so destroy
				Destroy();
			}
		}
	}
}

// Called every frame
void ASkillDropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

