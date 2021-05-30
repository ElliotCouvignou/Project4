// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4HubGameMode.h"
#include "Project4GameInstance.h"
#include "Project4PlayerState.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"
#include "P4CurrentGameSave.h"

#include "Characters/P4PlayerCharacterBase.h"



void AP4HubGameMode::ServerTravelToNewLevel_Implementation()
{
	SaveGameInfo();

	// TODO: figure out how to select random levels
	if (PlayableLevelNames.Num() > 0)
	{
		FString LevelName = PlayableLevelNames[(int)FMath::RandRange(0.f, (float)PlayableLevelNames.Num())];
		
		LevelName = LevelName.Append("?Game=/Game/Project4/Blueprints/GameModes/GM_OpenWorld.GM_OpenWorld_C");
		// server hosts as listen
		//LevelName.Append(FString("?listen"));
		ServerTravelToLevel(LevelName);
	}
	
}


void AP4HubGameMode::LoadFromPreGameLobby()
{
	UProject4GameInstance* GI = Cast<UProject4GameInstance>(GetGameInstance());
	if (GI->LoadFromPreGameLobbyInfo)
	{
		GI->LoadFromPreGameLobbyInfo = false;
		
		// TODO: other stuff if needed
	}
}

void AP4HubGameMode::LoadCharacterForClient(APlayerController* NewPlayer)
{
	Super::LoadCharacterForClient(NewPlayer);

}

void AP4HubGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);

	AProject4Controller* P4C = Cast<AProject4Controller>(Controller);
	if (P4C)
	{
		UE_LOG(LogTemp, Warning, TEXT("\n\n[AP4HubGameMode::GenericPlayerInitialization]   %s "), *Controller->GetName());
		LoadCharacterForClient(P4C);
		P4C->CreateMainHUDWidget();
	}
}

void AP4HubGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	/*AProject4Controller* P4C = Cast<AProject4Controller>(NewPlayer);
	if (P4C)
	{
		UE_LOG(LogTemp, Warning, TEXT("\n\n[AP4HubGameMode::GenericPlayerInitialization]   %s "), *NewPlayer->GetName());
		P4C->CreateMainHUDWidget();
	}*/
}

void AP4HubGameMode::StartPlay()
{
	Super::StartPlay();

	//LoadCharacterForClient(GetWorld()->GetFirstPlayerController());
	//
	//AProject4Controller* P4C = Cast<AProject4Controller>(GetWorld()->GetFirstPlayerController());
	//if (P4C)
	//{
	//	P4C->CreateMainHUDWidget();
	//}

	
	
}

void AP4HubGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);


	//UE_LOG(LogTemp, Warning, TEXT("\n\n[AP4HubGameMode::PostLogin]   %s "), *NewPlayer->GetName());
	//LoadCharacterForClient(NewPlayer);
	//
	//AProject4Controller* P4C = Cast<AProject4Controller>(NewPlayer);
	//if (P4C)
	//{
	//	P4C->CreateMainHUDWidget();
	//}
}

