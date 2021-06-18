// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/TeleporterActor.h"
#include "AI/P4DirectorActor.h"
#include "Project4GameMode.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

ATeleporterActor::ATeleporterActor()
{

}


void ATeleporterActor::OnInteract(const AP4PlayerCharacterBase* SourceActor)
{
	// TODO: start some vfx sequence

	// spawn Boss
	// TODO: implement spawn transfom, currently uses this current pos
	if(!bIsEnabled)
		SpawnBoss();
	else
	{
		AProject4GameMode* GM = GetWorld()->GetAuthGameMode<AProject4GameMode>();
		GM->ServerTravelToLevel(FString("/Game/Project4/Maps/HubLevel?listen?Game=/Game/Project4/Blueprints/GameModes/GM_Hub.GM_Hub_C"));
	}

}

void ATeleporterActor::SpawnBoss()
{
	AProject4GameMode* GM = GetWorld()->GetAuthGameMode<AProject4GameMode>();
	if (GM && GM->LevelToMobSpawnAsset)
	{
		FName CurrentLevelName = FName(GetWorld()->GetMapName());

		FMobSpawnInfo BossToSpawn;
		GM->LevelToMobSpawnAsset->GetRandomBossMob(CurrentLevelName, BossToSpawn);
		if (BossToSpawn.MobClass)
		{
			// DO Spawn TODO: figure out transform
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			Boss = GetWorld()->SpawnActor<AProject4Character>(BossToSpawn.MobClass, GetActorLocation(), FRotator(0.f, FMath::FRand(), 0.f), SpawnInfo);

			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, FName("OnBossDied"));
			Boss->OnCharacterDied.Add(Delegate);
		}
		
	}
}

void ATeleporterActor::OnBossDied(AProject4Character* Character)
{
	print(FString("Bossdied"));
	bIsEnabled = true;
}

void ATeleporterActor::BeginPlay()
{
}


