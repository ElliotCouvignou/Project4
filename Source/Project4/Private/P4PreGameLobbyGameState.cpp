// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4PreGameLobbyGameState.h"
#include "Project4Controller.h"
#include "UI/PreGameLobbyWidget.h"
#include "Project4PlayerState.h"
#include "Net/UnrealNetwork.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

AP4PreGameLobbyGameState::AP4PreGameLobbyGameState()
{
	
}

void AP4PreGameLobbyGameState::InitNewPlayerState(APlayerController* PC)
{
	if (PC)
	{
		PlayerNumberMap.Add(TTuple<APlayerController*, int>(PC, PlayerNumberMap.Num()));

		if (!PlayerInfo.Contains(PC))
			PlayerInfo.Add(TTuple<APlayerController*, FPreGameLobbyPlayerInfoStruct>(PC, FPreGameLobbyPlayerInfoStruct()));

		TArray<bool> States;
		for (auto e : PlayerInfo)
			States.Add(e.Value.PlayerReadyStatus);


		for (auto e : PlayerNumberMap)
		{
			AProject4PlayerState* PS = e.Key->GetPlayerState<AProject4PlayerState>();
			if (PS)
				PS->ClientInitPlayerReadyStates(States);
		}
	}
}

void AP4PreGameLobbyGameState::ChangeReadyPlayerStatus(const APlayerController* PC)
{
	if (PC)
	{
		FPreGameLobbyPlayerInfoStruct* Info = PlayerInfo.Find(PC);
		if (Info)
		{
			Info->PlayerReadyStatus = !Info->PlayerReadyStatus;

			for (auto e : PlayerNumberMap)
			{
				AProject4PlayerState* PS = e.Key->GetPlayerState<AProject4PlayerState>();
				if (PS)
					PS->ClientPlayerReadyStateChanged(*PlayerNumberMap.Find(PC), Info->PlayerReadyStatus);
			}
		}
	}
}





// ovveride replciation with replication variables
void AP4PreGameLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	//DOREPLIFETIME(AP4PreGameLobbyGameState, PlayerInfo);  Tmap replication not supported
}