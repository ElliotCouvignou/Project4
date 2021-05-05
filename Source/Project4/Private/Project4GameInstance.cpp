// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Project4GameInstance.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TextReaderComponent.h"


const static FName SESSION_NAME = TEXT("Project4GameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UProject4GameInstance::UProject4GameInstance()
{
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));

	APIUrl = TextReader->ReadFile("Urls/APIUrl.txt");
	RegionCode = TextReader->ReadFile("Urls/RegionCode.txt");
	HttpModule = &FHttpModule::Get();

	SettingsSave = Cast<UP4SettingsSave>(UGameplayStatics::CreateSaveGameObject(UP4SettingsSave::StaticClass()));

	//C: / Unreal Engine Projects / Project4 / Content / Project4 / UI / MainMenu / BP_MainMenuWidget.uasset
}

void UProject4GameInstance::Shutdown()
{
	GetWorld()->GetTimerManager().ClearTimer(RetrieveNewTokensHandle);
	GetWorld()->GetTimerManager().ClearTimer(GetLatencyTimeLandleHandle);

	// on shutdown, invalidate tokens generated on launch as we wont need them anymore
	if (AccessToken.Len() > 0)
	{
		if (MatchmakingTicketId.Len() > 0)
		{
			TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
			RequestObj->SetStringField("ticketId", MatchmakingTicketId);
			
			FString RequestBody;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
			if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
			{
				TSharedRef<IHttpRequest, ESPMode::ThreadSafe> StopMatchmakingRequest = HttpModule->CreateRequest();
				StopMatchmakingRequest->SetURL(APIUrl + "/stopmatchmaking");
				StopMatchmakingRequest->SetVerb("POST");
				StopMatchmakingRequest->SetHeader("Content-Type", "application/json");
				StopMatchmakingRequest->SetHeader("Authorization", AccessToken);
				StopMatchmakingRequest->SetContentAsString(RequestBody);
				StopMatchmakingRequest->ProcessRequest();
			}
		}

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> InvalidateTokensRequest = HttpModule->CreateRequest();
		InvalidateTokensRequest->SetURL(APIUrl + "/invalidatetokens");
		InvalidateTokensRequest->SetVerb("GET");
		InvalidateTokensRequest->SetHeader("Content-Type", "application/json");
		InvalidateTokensRequest->SetHeader("Authorization", AccessToken);
		InvalidateTokensRequest->ProcessRequest();
	}

	Super::Shutdown();
}

void UProject4GameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("P4GameInstance::Init(): Access Token: %s \n"), *AccessToken);

	// set timer to calc player latency (set to every second)
	//GetWorld()->GetTimerManager().SetTimer(GetLatencyTimeLandleHandle, this, &UProject4GameInstance::GetLatencyTime, 1.f, true, 0.f);

	// TODO: change index to save slot? this is settings per pc so prob not
	LoadSettingsData();

	// Setup online Subsystem stuff
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();
	if (SubSystem != nullptr)
	{
		SessionInterface = SubSystem->GetSessionInterface();

		//UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::Init] Found session interface"));

		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::Init] SessionInterface.IsValid"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UProject4GameInstance::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UProject4GameInstance::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UProject4GameInstance::OnFindSessionsComplete);

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UProject4GameInstance::OnJoinSessionsComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::Init] Found NO SUBSYSTEM"));
	}
}

void UProject4GameInstance::SetCognitoTokens(FString NewAccessToken, FString NewIdToken, FString NewRefreshToken)
{
	AccessToken = NewAccessToken;
	IdToken = NewIdToken;
	RefreshToken = NewRefreshToken;

	UE_LOG(LogTemp, Warning, TEXT("Access Token: %s \n"), *AccessToken);
	UE_LOG(LogTemp, Warning, TEXT("Refresh Token: %s \n"), *RefreshToken);

	GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UProject4GameInstance::RetrieveNewTokens, 1.0f, false, 3300.0f); //55 min timer
}

void UProject4GameInstance::LoadSettingsData()
{
	if (SettingsSaveSlot.Len() > 0)
	{
		SettingsSave = Cast<UP4SettingsSave>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlot, 0));
		if (SettingsSave == NULL)
		{
			// if failed to load, create a new one
			SettingsSave = Cast<UP4SettingsSave>(UGameplayStatics::CreateSaveGameObject(UP4SettingsSave::StaticClass()));
		}

		check(SettingsSave != NULL);
	}
}

void UProject4GameInstance::SaveSettingsData()
{
	UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlot, 0);
}

void UProject4GameInstance::LoadMainMenu()
{
	if (MenuClass == nullptr) return;

	MainMenu = CreateWidget<UMainMenuWidget>(this, MenuClass);
	MainMenu->AddToViewport(0);
}

void UProject4GameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		// Checks for an existing session
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::Host] There is an existing session about to remove the current one"));

			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::Host] About to create session"));

			// Create a new session
			CreateSession();

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::Host] SessionInterface invalid"));
	}
}

void UProject4GameInstance::JoinSession(const int Index)
{
	if (!SessionInterface.IsValid() || (!SessionSearch.IsValid())) 
		return;

	if (Index < SessionSearch->SearchResults.Num())
	{
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	}
}

void UProject4GameInstance::EndSession()
{
	// TODO: this, currently no reason to
}

void UProject4GameInstance::OpenSessionListMenu()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		print(FString("FindSessions"));
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OpenSessionListMenu] Session is valid"));
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 10000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}


void UProject4GameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	GetEngine()->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("[OnCreateSessionComplete::Host]"));

	UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::OnCreateSessionComplete] HOST TRAVEL TO LOBBY"));

	GetWorld()->ServerTravel("/Game/Project4/Maps/PreGameLobby?listen");
}

void UProject4GameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::OnDestroySessionComplete] Success "));
		CreateSession();
	}
	else
	{


		UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::OnDestroySessionComplete] NO Success "));
	}
}

void UProject4GameInstance::OnFindSessionsComplete(bool Success)
{
	print(FString("OnFindSessionsComplete"));

	if (Success && SessionSearch.IsValid())
	{
		if (SessionSearch->SearchResults.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] No Sessions Find"));
			print(FString("SessionSearch->SearchResults.Num() <= 0"));
		}
		else
		{
			TArray<FServerData> ServerData;
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Session Name %s"), *SearchResult.GetSessionIdStr());

				FServerData Data;
				FString ServerName;
				if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				{
					print(FString("Valid ServerName"));
					UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Data found in settings %s"), *ServerName);
					Data.Name = ServerName;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Data NOT found in settings"));

					print(FString("Could not find Valid ServerName"));
					Data.Name = "Could not find name";
				}

				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
				Data.HostUsername = SearchResult.Session.OwningUserName;

				ServerData.Add(Data);
			}

			print(FString("InitSessionsList"));
			MainMenu->InitializeSessionsList(ServerData);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Error session not found"));
	}
}

void UProject4GameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString Url;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Url))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::OnJoinSessionsComplete] Couldn't get Connect String"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::OnJoinSessionsComplete] Url: %s"), *Url);

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UProject4GameInstance::OnJoinSessionsComplete] Player Controller DOESN'T EXIST"));
		return;
	}


	PlayerController->ClientTravel(Url, ETravelType::TRAVEL_Absolute);
}

void UProject4GameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::CreateSession] Creating %s"), *SESSION_NAME.ToString());

	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		// Switch between bIsLANMatch when using NULL subsystem
		if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		// Number of sessions
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.NumPrivateConnections = 4;
		SessionSettings.NumPublicConnections = 4;
		//SessionSettings.NumPublicConnections = 4;
		//SessionSettings.bShouldAdvertise = true;
		//SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UProject4GameInstance::RetrieveNewTokens()
{
	if(AccessToken.Len() > 0 && RefreshToken.Len() > 0)
	{
		TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
		RequestObj->SetStringField("refreshToken", RefreshToken);

		FString RequestBody;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

		if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
		{
			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RetrieveNewTokensRequest = HttpModule->CreateRequest();
			RetrieveNewTokensRequest->OnProcessRequestComplete().BindUObject(this, &UProject4GameInstance::OnRetrieveNewTokensCallback);
			RetrieveNewTokensRequest->SetURL(APIUrl + "/retrievenewtokens");
			RetrieveNewTokensRequest->SetVerb("POST");
			RetrieveNewTokensRequest->SetHeader("Content-Type", "application/json");
			RetrieveNewTokensRequest->SetHeader("Authorization", AccessToken);
			RetrieveNewTokensRequest->SetContentAsString(RequestBody);
			RetrieveNewTokensRequest->ProcessRequest();
		}
		else {
			// in case we failed, try get tokens again
			GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UProject4GameInstance::RetrieveNewTokens, 1.0f, false, 30.0f);
		}
	}
}

void UProject4GameInstance::OnRetrieveNewTokensCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("accessToken") && JsonObject->HasField("idToken"))
			{
				// successful read, save tokens
				SetCognitoTokens(JsonObject->GetStringField("accessToken"), JsonObject->GetStringField("idToken"), RefreshToken);
			}
		}
		else
		{
			// Invalid, try again
			GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UProject4GameInstance::RetrieveNewTokens, 1.0f, false, 30.0f);
		}
	}
	else
	{
		// Invalid, try again
		GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UProject4GameInstance::RetrieveNewTokens, 1.0f, false, 30.0f);
	}
}

void UProject4GameInstance::GetLatencyTime()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GetLatencyTimeRequest = HttpModule->CreateRequest();
	GetLatencyTimeRequest->OnProcessRequestComplete().BindUObject(this, &UProject4GameInstance::OnGetLatencyTimeResponseRecieved);
	GetLatencyTimeRequest->SetURL("https://gamelift." + RegionCode + ".amazonaws.com");
	GetLatencyTimeRequest->SetVerb("GET");
	GetLatencyTimeRequest->ProcessRequest();
}

void UProject4GameInstance::OnGetLatencyTimeResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (PlayerLatencyTimes.Num() >= 4)
	{
		PlayerLatencyTimes.RemoveNode(PlayerLatencyTimes.GetHead());
	}

	float ResponseTime = Request->GetElapsedTime() * 1000; // convert to ms

	PlayerLatencyTimes.AddTail(ResponseTime);
}
