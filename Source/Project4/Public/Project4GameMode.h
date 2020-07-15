// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameModeBase.h"
#include "Project4GameMode.generated.h"


USTRUCT()
struct FStartGameSessionState
{
	GENERATED_BODY();

	UPROPERTY()
		bool Status;

	FStartGameSessionState() {
		Status = false;
	}
};

USTRUCT()
struct FUpdateGameSessionState
{
	GENERATED_BODY();

	FUpdateGameSessionState() {

	}
};

USTRUCT()
struct FProcessTerminateState
{
	GENERATED_BODY();

	UPROPERTY()
		bool Status;

	long TerminationTime;

	FProcessTerminateState() {
		Status = false;
	}
};

USTRUCT()
struct FHealthCheckState
{
	GENERATED_BODY();

	UPROPERTY()
		bool Status;

	FHealthCheckState() {
		Status = false;
	}
};


// Only those with authority can have access to gamemodes
// This includes dedicated servers and clients if they are doing 
// some offline mode or singleplayer

UCLASS(minimalapi)
class AProject4GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProject4GameMode();


	/***************************/
	/*      Player Death       */
	/***************************/

	// Called after FinishDying for Player character classes
	// starts respawn process 
	UFUNCTION()
		void PlayerDeath(AController* Controller);

	UFUNCTION()
		void RespawnPlayer(AController* Controller);


private:

	/***************************/
	/*      AWS Gamelift       */
	/***************************/

	// AWS stuff, Got it from tutorial lel 
	UPROPERTY()
		FStartGameSessionState StartGameSessionState;
	UPROPERTY()
		FUpdateGameSessionState UpdateGameSessionState;
	UPROPERTY()
		FProcessTerminateState ProcessTerminateState;
	UPROPERTY()
		FHealthCheckState HealthCheckState;

protected:
	/***************************/
	/*     Player Death        */
	/***************************/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float RespawnDelay = 5.f;

	/* Virtual Overrides */

	virtual void BeginPlay() override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
};



