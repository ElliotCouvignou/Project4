// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameMode.h"
#include "Interactables/SkillDropActor.h"
#include "AbilitySystem/P4AbilityPoolsDataAsset.h"
#include "Characters/P4CharacterInfoDataAsset.h"
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
class AProject4GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AProject4GameMode();

	// TODO: figure out wht to do with this if we wanna make classes for each char or just swap info
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TSubclassOf<class AP4PlayerCharacterBase> CharacterBaseClass;


	/***************************/
	/*      Player Death       */
	/***************************/

	// Called after FinishDying for Player character classes
	// starts respawn process 
	UFUNCTION()
		void PlayerDeath(AController* Controller);

	UFUNCTION()
		void RespawnPlayer(AController* Controller);

	UFUNCTION(BlueprintCallable, BlueprintCallable)
		void ServerTravelToLevel(const FString& LevelName);


	UFUNCTION(BlueprintCallable)
		virtual void SaveGameInfo();

	UFUNCTION(BlueprintCallable)
		virtual void LoadCharacterForClient(APlayerController* NewPlayer);

	UFUNCTION(BlueprintCallable)
		virtual void LoadCurrentGameInfo();

	UFUNCTION(BlueprintCallable)
		void CreateCharacter(EClassAbilityPoolType CharClass, APlayerController* OwningPC);

	UFUNCTION(BlueprintCallable)
		virtual void SetCharacterSpawnPosition(AP4PlayerCharacterBase* PChar);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities | Pools")
		UP4AbilityPoolsDataAsset* GlobalAbilityPoolDataAsset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UP4CharacterInfoDataAsset* PoolCharacterInfoMap;

	/* map of enum to character class to spawn */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap<EClassAbilityPoolType, TSubclassOf<AP4PlayerCharacterBase>> ClassToCharClass;

	virtual void PostSeamlessTravel() override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void GenericPlayerInitialization(AController* Controller) override;


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



