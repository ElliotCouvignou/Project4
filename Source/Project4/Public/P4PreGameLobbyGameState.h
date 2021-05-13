// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Characters/P4CharacterInfoDataAsset.h"
#include "P4PreGameLobbyGameState.generated.h"


USTRUCT(BlueprintType)
struct FPreGameLobbyPlayerInfoStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EClassAbilityPoolType PlayerChosenPool;

	UPROPERTY(BlueprintReadWrite)
		bool PlayerReadyStatus;

	FPreGameLobbyPlayerInfoStruct()
	{
		PlayerChosenPool = EClassAbilityPoolType::None;
		PlayerReadyStatus = false;
	}
};

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4PreGameLobbyGameState : public AGameState
{
	GENERATED_BODY()


public:

	AP4PreGameLobbyGameState();

	/* map of player controller index to ready status */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		TMap<APlayerController*, FPreGameLobbyPlayerInfoStruct> PlayerInfo;

	/* Index of each player controller represents their player # */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TMap<APlayerController*, int> PlayerNumberMap;

	/* returns -1 if invalid */
	UFUNCTION(BlueprintCallable)
		int GetPlayerIndex(APlayerController* PC) { return (PlayerNumberMap.Contains(PC) ? *PlayerNumberMap.Find(PC) : -1); }

	UFUNCTION(BlueprintCallable)
		void InitNewPlayerState(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
		void ChangeReadyPlayerStatus(const APlayerController* PC);




	


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};

