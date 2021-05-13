// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4PreGameLobbyGameMode.h"
#include "Project4Controller.h"
#include "Project4GameInstance.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "P4PreGameLobbyGameState.h"
#include "UI/PreGameLobbyWidget.h"
#include "Project4PlayerState.h"
#include "Kismet/GameplayStatics.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AP4PreGameLobbyGameMode::AP4PreGameLobbyGameMode()
	: Super()
{
	bUseSeamlessTravel = true;

}

void AP4PreGameLobbyGameMode::OnPlayerCharacterSelected(APlayerController* PC, const EClassAbilityPoolType& PoolType)
{
	print(FString("OnPlayerCharacterSelected"));
	if (!PC || PoolType == EClassAbilityPoolType::None || !PoolCharacterInfoMap)
		return;

	AP4PreGameLobbyGameState* GS = GetGameState<AP4PreGameLobbyGameState>();
	if (GS)
	{
		FPreGameLobbyPlayerInfoStruct* Info = GS->PlayerInfo.Find(PC);
		if (Info)
			Info->PlayerChosenPool = PoolType;
		print(FString("1"));
		// create player. TODO: determine if we need to subclass in bp OR just expand on data asset to swap info
		// currently swapping info over creating new character and swapping possession


		AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(PC->GetPawn());
		if (PChar)
		{
			print(FString("3"));
			//FCharacterInfoStruct* CharInfo = PoolCharacterInfoMap->PoolTypeToInfoMap.Find(PoolType);
			//PChar->ServerSetCharacterInfo(*CharInfo, PoolType);
			//PChar->MulticastSetCharacterInfo(*CharInfo);

			PChar->Destroy();
		}

		CreateCharacter(PoolType, PC);
	}
}

void AP4PreGameLobbyGameMode::OnPlayerReadyStatusChanged(const APlayerController* PC)
{
	print(FString("OnPlayerReadyStatusChanged"));

	AP4PreGameLobbyGameState* GS = GetGameState<AP4PreGameLobbyGameState>();
	if (GS && GS->PlayerInfo[PC].PlayerChosenPool != EClassAbilityPoolType::None)
	{
		GS->ChangeReadyPlayerStatus(PC);

		print(FString("1"));
		// determine all ready status
		for (auto Elem : GS->PlayerInfo)
		{
			FPreGameLobbyPlayerInfoStruct& Info = Elem.Value;
			if (!Info.PlayerReadyStatus)
				return;
		}

		print(FString("All Players Ready"));
		// if we made it here everyone is ready
		// save info to game instance for when we done travel
		UProject4GameInstance* GI = Cast<UProject4GameInstance>(GetGameInstance());
		GI->LoadFromPreGameLobbyInfo = true;

		// TODO: save current game info into game 
		GI->CreateNewCurrentGameData();
		SaveGameInfo();

		TArray<AActor*> Actors;
		GetSeamlessTravelActorList(true, Actors);
		print(FString("SealessTravelActorList: "));
		for (AActor* a : Actors)
		{
			print(a->GetDebugName(a));
		}

		// Remove PregameLobbyWidget from clients
		// TODO maybe move this to clientside to detect and handle over RPC
		for (auto e : GS->PlayerNumberMap)
		{
			AProject4Controller* P4C = Cast<AProject4Controller>(e.Key);
			if (P4C)
			{
				P4C->ClearWidgets();
			}
		}
		//GetWorld()->SeamlessTravel("/Game/Project4/Maps/HubLevel?listen");
		GetWorld()->GetTimerManager().SetTimer(PlayToLoadTimerHandle, this, &AP4PreGameLobbyGameMode::TravelToHub, 2.f, false, 2.f);
	
	}
	// multicast to clients to connect to session
}



void AP4PreGameLobbyGameMode::TravelToHub()
{
	if (!GetWorld()->ServerTravel("/Game/Project4/Maps/HubLevel?listen?Game=/Game/Project4/Blueprints/GameModes/GM_Hub.GM_Hub_C"))
	{
		print(FString("Failed ServerTravvel"));
	}
}

void AP4PreGameLobbyGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	print(FString("AP4PreGameLobbyGameMode::PostSeamless"));

}

void AP4PreGameLobbyGameMode::StartPlay()
{
	Super::StartPlay();

	AProject4Controller* P4C = Cast<AProject4Controller>(GetWorld()->GetFirstPlayerController());
	if (P4C)
	{
		P4C->CreatePreGameLobbyWidget();
	}

}


void AP4PreGameLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority() && NewPlayer)
	{
		//AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(NewPlayer->GetPawn());
		//if (!PChar)
		//{
		//	print(FString("2"));
		//	// TODO: set this locaiton out of camera, then multicast the clients to place the locaiton of this char to their needed area
		//	PChar = GetWorld()->SpawnActorDeferred<AP4PlayerCharacterBase>(DefaultCharacterClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//	PChar->SetOwner(NewPlayer);
		//	NewPlayer->Possess(PChar);
		//	NewPlayer->SetPawn(PChar);
		//	PChar->FinishSpawning(FTransform());
		//
		//
		//	PChar->Mutlicast_SetPreGameLobbyPosition();
		//}

		AProject4Controller* P4C = Cast<AProject4Controller>(NewPlayer);
		if (P4C)
		{
			P4C->CreatePreGameLobbyWidget();
		}
		PossessPreGameLobbyCamera(NewPlayer);

	}
	
}



