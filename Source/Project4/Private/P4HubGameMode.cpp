// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4HubGameMode.h"
#include "Project4GameInstance.h"
#include "Project4PlayerState.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"
#include "Characters/P4PlayerCharacterBase.h"





void AP4HubGameMode::ServerTravelToLevel_Implementation(const FString& LevelName)
{
	GetWorld()->ServerTravel(LevelName);
}

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

void AP4HubGameMode::BeginPlay()
{
	UProject4GameInstance* GI = Cast<UProject4GameInstance>(GetGameInstance());

	// Check if we are loading from pregame lobby
	if (GI->LoadFromPreGameLobbyInfo)
	{
		//FPreGameLobbyInfoStruct& Info = GI->PreGameLobbyInfo;
		//if (Info.PlayerInfoMap.Num() == 0)
		//{
		//	// TODO something idk if i should even bother with this case (prob from playing in editor and skipping pregame)
		//}
		//else
		//{
		//	// Create Characters and set the player's chosen , assign mapped player controller to created character
		//	for (auto elem : Info.PlayerInfoMap)
		//	{
		//		AP4PlayerCharacterBase* PChar = GetWorld()->SpawnActorDeferred<AP4PlayerCharacterBase>(CharacterBaseClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//		PChar->SetOwner(elem.Key);
		//		elem.Key->SetPawn(PChar);
		//
		//		AProject4PlayerState* PS = elem.Key->GetPlayerState<AProject4PlayerState>();
		//		if (PS)
		//		{
		//			UP4PlayerAbilitySystemComponent* ASC = Cast<UP4PlayerAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		//			if (ASC)
		//			{
		//				ASC->Server_OnAbilityPoolPicked_Implementation(elem.Value.PlayerChosenPool);
		//			}
		//		}
		//
		//		// TODO: find location for tihs
		//		PChar->FinishSpawning(FTransform());
		//	}
		//}
	}
	

}
