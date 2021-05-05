// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project4.h"
#include "Characters/P4CharacterInfoDataAsset.h"
#include "P4PreGameLobbyGameMode.generated.h"



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

USTRUCT(BlueprintType)
struct FPreGameLobbyInfoStruct
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TMap <APlayerController*, FPreGameLobbyPlayerInfoStruct> PlayerInfoMap;

	FPreGameLobbyInfoStruct()
	{
		
	}

	FPreGameLobbyInfoStruct(const FPreGameLobbyInfoStruct& other)
	{
		PlayerInfoMap = other.PlayerInfoMap;
	}
};

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4PreGameLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
		void OnPlayerCharacterSelected(const APlayerController* PC, const EClassAbilityPoolType& PoolType);

	UFUNCTION(BlueprintCallable)
		void OnPlayerReadyStatusChanged(const APlayerController* PC);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PossessPreGameLobbyCamera(const APlayerController* PC);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		FPreGameLobbyInfoStruct PreGameInfo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UP4CharacterInfoDataAsset* PoolCharacterInfoMap;

	/* Class to spawn when player finally selects */
	// TODO: determine to make this map if swapping spawns new chars
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<ACharacter> DefaultCharacterClass;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
