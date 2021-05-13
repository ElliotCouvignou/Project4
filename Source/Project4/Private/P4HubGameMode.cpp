// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4HubGameMode.h"
#include "Project4GameInstance.h"
#include "Project4PlayerState.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"
#include "Characters/P4PlayerCharacterBase.h"



void AP4HubGameMode::ServerTravelToNewLevel_Implementation()
{
	// TODO: figure out how to select random levels
	if (PlayableLevelNames.Num() > 0)
	{
		FString LevelName = PlayableLevelNames[(int)FMath::RandRange(0.f, (float)PlayableLevelNames.Num())];
		
		// server hosts as listen
		//LevelName.Append(FString("?listen"));
		ServerTravelToLevel(LevelName);
	}
	
}

void AP4HubGameMode::StartPlay()
{
	Super::StartPlay();

	LoadCurrentGameInfo();

	AProject4Controller* P4C = Cast<AProject4Controller>(GetWorld()->GetFirstPlayerController());
	if (P4C)
	{
		P4C->CreateMainHUDWidget();
	}
}

void AP4HubGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	AProject4Controller* P4C = Cast<AProject4Controller>(NewPlayer);
	if (P4C)
	{
		P4C->CreateMainHUDWidget();
	}
}

