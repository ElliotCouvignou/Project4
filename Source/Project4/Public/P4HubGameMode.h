// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Project4GameMode.h"
#include "P4HubGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4HubGameMode : public AProject4GameMode
{
	GENERATED_BODY()
	

public:




	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TArray<FString> PlayableLevelNames;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, BlueprintCallable)
		void ServerTravelToNewLevel();
	void ServerTravelToNewLevel_Implementation();
	bool ServerTravelToNewLevel_Validate() { return true; }

	/* Funciton called from levelblueprints since no way to go from here to there */
	UFUNCTION(BlueprintCallable)
		void LoadFromPreGameLobby();

	virtual void LoadCharacterForClient(APlayerController* NewPlayer) override;

	virtual void GenericPlayerInitialization(AController* Controller) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
