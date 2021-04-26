// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P4HubGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4HubGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TArray<FString> PlayableLevelNames;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, BlueprintCallable)
		void ServerTravelToLevel(const FString& LevelName);
	void ServerTravelToLevel_Implementation(const FString& LevelName);
	bool ServerTravelToLevel_Validate(const FString& LevelName) { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, BlueprintCallable)
		void ServerTravelToNewLevel();
	void ServerTravelToNewLevel_Implementation();
	bool ServerTravelToNewLevel_Validate()  { return true; }

};
