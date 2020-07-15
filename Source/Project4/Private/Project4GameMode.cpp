// Copyright Epic Games, Inc. All Rights Reserved.

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "Project4GameMode.h"
#include "Project4Controller.h"
#include "Project4PlayerState.h"
#include "Characters/Project4Character.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AProject4GameMode::AProject4GameMode()
{
	bUseSeamlessTravel = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Project4/Characters/Player/BP_ALS_PlayerBase"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerStateClass = AProject4PlayerState::StaticClass();
	}
}

void AProject4GameMode::PlayerDeath(AController* Controller)
{
	// place to spectator cam and respawn after delay
	//FActorSpawnParameters SpawnParameters;
	//SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);
	//
	//
	//Controller->UnPossess();
	//Controller->Possess(SpectatorPawn);

	// forced respawns on timer finished
	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &AProject4GameMode::RespawnPlayer, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);

	AProject4Controller* PC = Cast<AProject4Controller>(Controller);
	if (PC)
	{
		PC->SetUIRespawnCountdown(RespawnDelay);
	}
}

void AProject4GameMode::RespawnPlayer(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		AActor* PlayerStart = FindPlayerStart(Controller);

		AProject4Controller* PC = Cast<AProject4Controller>(Controller);
		AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(PC->GetPawn());
		if (PChar)
		{
			PChar->SetActorTransform(PlayerStart->GetTransform());
			PC->SetIgnoreMoveInput(false);
			PChar->Respawn();

		}

		// remove spectator pawn and replace with new player actor
		//APawn* OldSpectatorPawn = Controller->GetPawn();
		//Controller->UnPossess();
		//OldSpectatorPawn->Destroy();
		//Controller->Possess(Player);
	}
	else
	{
		// AI Controller of player class 
		// only do if we want bots of Players
	}
}

void AProject4GameMode::BeginPlay()
{
	Super::BeginPlay();
	//print(FString(TEXT("ALLO")));

// run on server only (do some diagnostic checks and handle issues w/force quit //
#if WITH_GAMELIFT
	auto InitSDKOutcome = Aws::GameLift::Server::InitSDK();



	if (InitSDKOutcome.IsSuccess()) {
		auto OnStartGameSession = [](Aws::GameLift::Server::Model::GameSession GameSessionObj, void* Params)
		{
			FStartGameSessionState* State = (FStartGameSessionState*)Params;

			State->Status = Aws::GameLift::Server::ActivateGameSession().IsSuccess();
		};

		auto OnUpdateGameSession = [](Aws::GameLift::Server::Model::UpdateGameSession UpdateGameSessionObj, void* Params)
		{
			FUpdateGameSessionState* State = (FUpdateGameSessionState*)Params;
		};

		auto OnProcessTerminate = [](void* Params)
		{
			FProcessTerminateState* State = (FProcessTerminateState*)Params;

			auto GetTerminationTimeOutcome = Aws::GameLift::Server::GetTerminationTime();
			if (GetTerminationTimeOutcome.IsSuccess()) {
				State->TerminationTime = GetTerminationTimeOutcome.GetResult();
			}

			auto ProcessEndingOutcome = Aws::GameLift::Server::ProcessEnding();

			if (ProcessEndingOutcome.IsSuccess()) {
				State->Status = true;
				FGenericPlatformMisc::RequestExit(false);
			}
		};

		auto OnHealthCheck = [](void* Params)
		{
			FHealthCheckState* State = (FHealthCheckState*)Params;
			State->Status = true;

			return State->Status;
		};

		TArray<FString> CommandLineTokens;   // holds prelim flags after Project2.exe before - flags
		TArray<FString> CommandLineSwitches; // holds - flags
		int Port = FURL::UrlConfig.DefaultPort;

		// Project2.exe token -port=7777
		FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);

		for (FString Str : CommandLineSwitches) {
			FString Key;
			FString Value;

			if (Str.Split("=", &Key, &Value)) {
				if (Key.Equals("port")) {
					Port = FCString::Atoi(*Value);
				}
			}
		}

		const char* LogFile = "aLogFile.txt"; // save logfile
		const char** LogFiles = &LogFile;
		auto LogParams = new Aws::GameLift::Server::LogParameters(LogFiles, 1);

		auto Params = new Aws::GameLift::Server::ProcessParameters(
			OnStartGameSession,
			&StartGameSessionState,
			OnUpdateGameSession,
			&UpdateGameSessionState,
			OnProcessTerminate,
			&ProcessTerminateState,
			OnHealthCheck,
			&HealthCheckState,
			Port,
			*LogParams
		);

		auto ProcessReadyOutcome = Aws::GameLift::Server::ProcessReady(*Params);
		// TODO handle when ProcessReady returns false (rare, but it happens)
	}
#endif
}

FString AProject4GameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	/*if (NewPlayerController != nullptr) {
		APlayerState* PlayerState = NewPlayerController->PlayerState;
		if (PlayerState != nullptr) {
			AGameLiftTutorialPlayerState* GameLiftTutorialPlayerState = Cast<AGameLiftTutorialPlayerState>(PlayerState);
			if (GameLiftTutorialPlayerState != nullptr) {
				if (FMath::RandRange(0, 1) == 0) {
					GameLiftTutorialPlayerState->Team = "cowboys";
				}
				else {
					GameLiftTutorialPlayerState->Team = "aliens";
				}
			}
		}
	}*/
	return InitializedString;
}
