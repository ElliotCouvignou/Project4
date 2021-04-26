// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4HubGameMode.h"





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
