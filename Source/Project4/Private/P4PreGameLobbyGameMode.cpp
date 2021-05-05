// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4PreGameLobbyGameMode.h"
#include "Project4Controller.h"
#include "Project4GameInstance.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

void AP4PreGameLobbyGameMode::OnPlayerCharacterSelected(const APlayerController* PC, const EClassAbilityPoolType& PoolType)
{
	print(FString("OnPlayerCharacterSelected"));
	if (!PC || PoolType == EClassAbilityPoolType::None || !PoolCharacterInfoMap)
		return;

	FPreGameLobbyPlayerInfoStruct* Info = PreGameInfo.PlayerInfoMap.Find(PC);
	if (Info)
		Info->PlayerChosenPool = PoolType;
	print(FString("1"));
	// create player. TODO: determine if we need to subclass in bp OR just expand on data asset to swap info
	// currently swapping info over creating new character and swapping possession


	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(PC->GetPawn());
	if (PChar)
	{
		print(FString("3"));
		FCharacterInfoStruct& CharInfo = *PoolCharacterInfoMap->PoolTypeToInfoMap.Find(PoolType);
		PChar->MulticastSetCharacterInfo(CharInfo);
	}
}

void AP4PreGameLobbyGameMode::OnPlayerReadyStatusChanged(const APlayerController* PC)
{
	print(FString("OnPlayerReadyStatusChanged"));
	FPreGameLobbyPlayerInfoStruct* PlayerInfo = PreGameInfo.PlayerInfoMap.Find(PC);
	if (PlayerInfo)
	{
		PlayerInfo->PlayerReadyStatus = !PlayerInfo->PlayerReadyStatus;
	}
	print(FString("1"));
	// determine all ready status
	for (auto Elem : PreGameInfo.PlayerInfoMap)
	{
		FPreGameLobbyPlayerInfoStruct& Info = Elem.Value;
		if (!Info.PlayerReadyStatus)
			return;
	}
	
	print(FString("All Players Ready"));
	// if we made it here everyone is ready
	// save info to game instance for when we done travel
	UProject4GameInstance* GI = Cast<UProject4GameInstance>(GetGameInstance());
	//GI->PreGameLobbyInfo = PreGameInfo;
	GI->LoadFromPreGameLobbyInfo = true;
	//GetWorld()->ServerTravel("/Game/Project4/Maps/HubLevel?listen");

	UGameplayStatics::OpenLevel(GetWorld(), FName(*FString("HubLevel")), true, "listen");

	// multicast to clients to connect to session
}

void AP4PreGameLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (HasAuthority())
	{
		if (NewPlayer)
		{
			if (PreGameInfo.PlayerInfoMap.Contains(NewPlayer))
				return;

			PreGameInfo.PlayerInfoMap.Add(TTuple<APlayerController*, FPreGameLobbyPlayerInfoStruct>(NewPlayer, FPreGameLobbyPlayerInfoStruct()));
		}

		AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(NewPlayer->GetPawn());
		if (!PChar)
		{
			print(FString("2"));
			// TODO: set this locaiton out of camera, then multicast the clients to place the locaiton of this char to their needed area
			PChar = GetWorld()->SpawnActorDeferred<AP4PlayerCharacterBase>(DefaultCharacterClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			PChar->SetOwner(NewPlayer);
			NewPlayer->SetPawn(PChar);
			PChar->FinishSpawning(FTransform());


			PChar->Mutlicast_SetPreGameLobbyPosition();
		}

		AProject4Controller* P4C = Cast<AProject4Controller>(NewPlayer);
		if (P4C)
		{
			P4C->CreatePreGameLobbyWidget();
		}
		PossessPreGameLobbyCamera(NewPlayer);
	}
	
}



