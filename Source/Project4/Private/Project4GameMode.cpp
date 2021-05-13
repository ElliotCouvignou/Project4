// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project4GameMode.h"
#include "Project4Controller.h"
#include "Project4PlayerState.h"
#include "P4PreGameLobbyGameState.h"
#include "Project4GameInstance.h"

#include "Characters/Project4Character.h"
#include "Characters/P4PlayerCharacterBase.h"

#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"

#include "Interactables/P4InventoryBagComponent.h"

#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

#include "Blueprint/WidgetBlueprintLibrary.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

AProject4GameMode::AProject4GameMode()
{
	bUseSeamlessTravel = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Project4/Characters/Player/P4ALSPlayerChar"));
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

void AProject4GameMode::ServerTravelToLevel(const FString& LevelName)
{
	// TODO: maybe move saving current game info logic here
	GetWorld()->ServerTravel(LevelName);
}


void AProject4GameMode::SaveGameInfo()
{
	print(FString("SaveGameInfo()"));
	UProject4GameInstance* GI = Cast<UProject4GameInstance>(GetGameInstance());
	AP4PreGameLobbyGameState* GS = GetGameState<AP4PreGameLobbyGameState>();
	UP4CurrentGameSave* Save = GI->CurrentGameSave;
	if (GS && Save)
	{
		// Iterate through player controllers, gather character and send to save object ot parse
		for (auto e : GS->PlayerNumberMap)
		{
			Save->SaveCharacterInfoFromPlayer(Cast<AProject4Controller>(e.Key), e.Key->GetPawn<AP4PlayerCharacterBase>());
		}
	}
}

void AProject4GameMode::LoadCurrentGameInfo()
{
	UProject4GameInstance* GI = Cast<UProject4GameInstance>(GetGameInstance());
	UP4CurrentGameSave* Save = GI->CurrentGameSave;
	AP4PreGameLobbyGameState* GS = GetGameState<AP4PreGameLobbyGameState>();

	print(FString("\n\nAProject4GameMode::LoadCurrentGameInfo"));
	if (Save)
	{
		print(FString("ValidSave"));
		// Unload info for each player in order it was stored in (order isn't actually relevant)
		for (auto e : Save->PlayerCharacterSave)
		{
			// controller should always be p4
			FP4CharacterSaveStruct* PSave = GI->CurrentGameSave->PlayerCharacterSave.Find(Cast<AProject4Controller>(e.Key));
		
			if (PSave)
			{
				// 0. Player character class defaults (Mesh, Init att set, etc.)
				print(FString("0"));
				//FCharacterInfoStruct* CharInfo = PoolCharacterInfoMap->PoolTypeToInfoMap.Find(PSave->PlayerClass);
				//PChar->ServerSetCharacterInfo(*CharInfo, PSave->PlayerClass);
				//PChar->MulticastSetCharacterInfo(*CharInfo);
				CreateCharacter(PSave->PlayerClass, e.Key);
				AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(e.Key->GetPawn());

				if (!PChar)
				{
					UE_LOG(LogTemp, Warning, TEXT("AProject4GameMode::LoadCurrentGameInfo] Invalid PChar from getPawn()"));
				}

				// 1. Essential non-derived Attributes
				print(FString("1"));
				UPlayerAttributeSet* AS = Cast<UPlayerAttributeSet>(PChar->GetAttributeSet());
				if (AS)
				{
					AS->SetLevel(*PSave->AttributeSaves.Find("Level"));
					AS->SetExperience(*PSave->AttributeSaves.Find("Experience"));
				}

				// 2. InventoryItems and Equips
				print(FString("2"));
				UP4InventoryBagComponent* IBC = PChar->GetInventoryBagComponent();
				if (IBC)
				{
					// Inventory Items
					for (auto b : PSave->InventorySaves)
						IBC->InventoryArray[b.InventoryIndex] = b.ItemObject;
					
					// Player Equips
					IBC->EquippmentSlots = PSave->PlayerEquips;
				}

				// 3. Relevant Gameplay Effects
				print(FString("3"));
				UAbilitySystemComponent* ASC = PChar->GetAbilitySystemComponent();
				if (ASC)
				{
					for (auto b : PSave->GameplayEffects)
						ASC->ApplyGameplayEffectSpecToSelf(b);
				}
			}	
		}
	}
	print(FString("end\n"));
}

void AProject4GameMode::CreateCharacter(EClassAbilityPoolType CharClass, APlayerController* OwningPC)
{
	print(FString("AProject4GameMode::CreateCharacter()\n"));
	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(OwningPC->GetPawn());
	if (!PChar)
	{
		print(FString("Creating New Character"));
		// TODO: set this locaiton out of camera, then multicast the clients to place the locaiton of this char to their needed area
		PChar = GetWorld()->SpawnActorDeferred<AP4PlayerCharacterBase>(*ClassToCharClass.Find(CharClass), FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		PChar->SetOwner(OwningPC);
		OwningPC->Possess(PChar);
		OwningPC->SetPawn(PChar);
		PChar->SetPlayerState(OwningPC->GetPlayerState<AProject4PlayerState>());
		PChar->FinishSpawning(FTransform());


		PChar->Mutlicast_SetPreGameLobbyPosition();

		UP4PlayerAbilitySystemComponent* ASC = Cast< UP4PlayerAbilitySystemComponent>(OwningPC->GetPlayerState<AProject4PlayerState>()->GetAbilitySystemComponent());
		//UP4PlayerAbilitySystemComponent* ASC = Cast< UP4PlayerAbilitySystemComponent>(PChar->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->Server_OnAbilityPoolPicked(CharClass);
		}
	}
}

void AProject4GameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	
}

void AProject4GameMode::StartPlay()
{
	Super::StartPlay();

	// TODO: change class type to base one if i do that eventually
	AP4PreGameLobbyGameState* GS = GetGameState<AP4PreGameLobbyGameState>();
	if (GS)
	{
		GS->InitNewPlayerState(0);
	}

}

void AProject4GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// TODO: this one too
	AP4PreGameLobbyGameState* GS = GetGameState<AP4PreGameLobbyGameState>();
	if (GS)
	{
		GS->InitNewPlayerState(NewPlayer);
	}


}

void AProject4GameMode::BeginPlay()
{
	Super::BeginPlay();

	

	//print(FString(TEXT("ALLO")));
	UE_LOG(LogTemp, Warning, TEXT("GameliftServer::BeginPlay() start, WITH_GAMELIFT=%i"), WITH_GAMELIFT);

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
