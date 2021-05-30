// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Project4GameMode.h"
#include "Project4.h"
#include "P4PreGameLobbyGameMode.generated.h"





//USTRUCT(BlueprintType)
//struct FPreGameLobbyInfoStruct
//{
//	GENERATED_USTRUCT_BODY()
//
//		UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
//		TMap <APlayerController*, FPreGameLobbyPlayerInfoStruct> PlayerInfoMap;
//
//	FPreGameLobbyInfoStruct()
//	{
//		
//	}
//
//	FPreGameLobbyInfoStruct(const FPreGameLobbyInfoStruct& other)
//	{
//		PlayerInfoMap = other.PlayerInfoMap;
//	}
//};

// used as state machine types in gamemode for seamless travel use (seamless travel doesn't change gamemodes)
UENUM(BlueprintType)
enum class EP4GameModeStateType : uint8
{
	OutOfRange			UMETA(DisplayName = "OutOfRange"),
	OutOfMana			UMETA(DisplayName = "OutOfMana"),
	NoTarget			UMETA(DisplayName = "NoTarget"),
	InvalidTarget       UMETA(DisplayName = "InvalidTarget"),
	InvalidState		UMETA(DisplayName = "InvalidState"),
	OnCooldown			UMETA(DisplayName = "OnCooldown")
};

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4PreGameLobbyGameMode : public AProject4GameMode
{
	GENERATED_BODY()


public:
	AP4PreGameLobbyGameMode();


	UFUNCTION(BlueprintCallable)
		void OnPlayerCharacterSelected(APlayerController* PC, const EClassAbilityPoolType& PoolType);

	UFUNCTION(BlueprintCallable)
		void OnPlayerReadyStatusChanged(const APlayerController* PC);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PossessPreGameLobbyCamera(const APlayerController* PC);



	UFUNCTION()
		void TravelToHub();

	/* Class to spawn when player finally selects */
	// TODO: determine to make this map if swapping spawns new chars
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<ACharacter> DefaultCharacterClass;


	FTimerHandle PlayToLoadTimerHandle;

	virtual void SetCharacterSpawnPosition(AP4PlayerCharacterBase* PChar) override;

	virtual void PostSeamlessTravel() override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
