// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/SkillDropActor.h"
#include "Project4GameMode.h"

// Sets default values
ASkillDropActor::ASkillDropActor() : Super()
{
	bReplicates = false;   // Each player gets their own drops so don't replicate
}

// Called when the game starts or when spawned
void ASkillDropActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkillDropActor::Server_OnPlayerSkillDropInteracted_Implementation(const AP4PlayerCharacterBase* Actor)
{
	// roll random abilities
}

void ASkillDropActor::OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result)
{
	AProject4GameMode* GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		Server_OnPlayerSkillDropInteracted(SourceActor);
		Result = true;
	}
}

// Called every frame
void ASkillDropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

