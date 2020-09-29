// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/MainMenuWidget.h"
#include "WebBrowser.h"
#include "WebBrowserModule.h"
#include "IWebBrowserSingleton.h"
#include "IWebBrowserCookieManager.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "TextReaderComponent.h"
#include "Project4GameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));

	LoginUrl = TextReader->ReadFile("Urls/LoginUrl.txt");
	APIUrl = TextReader->ReadFile("Urls/APIUrl.txt");
	CallbackUrl = TextReader->ReadFile("Urls/CallbackUrl.txt");
	RegionCode = TextReader->ReadFile("Urls/RegionCode.txt");

	HttpModule = &FHttpModule::Get();

	PlayerLatency = 100000.f;

	SearchingForGame = false;
}


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	WebBrowser = (UWebBrowser*)GetWidgetFromName(TEXT("WebBrowser_Login"));
	FindGameButton = (UButton*)GetWidgetFromName(TEXT("Button_FindGame"));
	FScriptDelegate FindGameDelegate;
	FindGameDelegate.BindUFunction(this, "OnFindGameButtonClicked");
	FindGameButton->OnClicked.Add(FindGameDelegate);

	CharacterNameTextBlock = (UTextBlock*)GetWidgetFromName(TEXT("CharacterNameText"));
	CharacterLevelTextBlock = (UTextBlock*)GetWidgetFromName(TEXT("CharacterLevelText"));
	PingTextBlock = (UTextBlock*)GetWidgetFromName(TEXT("PingText"));
	MatchMakingEventTextBlock = (UTextBlock*)GetWidgetFromName(TEXT("MatchmakingEventText"));
	MainMenuVBox = (UVerticalBox*)GetWidgetFromName(TEXT("RealMenuBox"));

		// poll ping #'s
	GetWorld()->GetTimerManager().SetTimer(SetPlayerLatencyHandle, this, &UMainMenuWidget::SetPlayerLatency, 1.0f, true, 1.0f);

	FString AccessToken;
	UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
	if (P4GameInstance)
	{
		AccessToken = P4GameInstance->AccessToken;
	}

	// pre-existing accesstoken on construct... try get player data
	if (AccessToken.Len() > 0)
	{
		TSharedRef<IHttpRequest> GetPlayerDataRequest = HttpModule->CreateRequest();
		GetPlayerDataRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnGetPlayerDataResponseRecieved);
		GetPlayerDataRequest->SetURL(APIUrl + "/getplayerdata");
		GetPlayerDataRequest->SetVerb("GET");
		GetPlayerDataRequest->SetHeader("Content-Type", "application/json");
		GetPlayerDataRequest->SetHeader("Authorization", AccessToken);
		GetPlayerDataRequest->ProcessRequest();
	}
	else
	{
		// get accesstoken through Login
		IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
		if (WebBrowserSingleton)
		{
			TOptional<FString> DefaultContext;
			TSharedPtr<IWebBrowserCookieManager> CookieManager = WebBrowserSingleton->GetCookieManager(DefaultContext);
			if (CookieManager.IsValid())
			{
				CookieManager->DeleteCookies();
			}
		}

		WebBrowser->LoadURL(LoginUrl);

		FScriptDelegate LoginDelegate;
		LoginDelegate.BindUFunction(this, "HandleLoginUrlChange");
		WebBrowser->OnUrlChanged.Add(LoginDelegate);
	}
}

void UMainMenuWidget::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(PollMatchmakingHandle);
	GetWorld()->GetTimerManager().ClearTimer(SetPlayerLatencyHandle);

	Super::NativeDestruct();
}

void UMainMenuWidget::HandleLoginUrlChange()
{
	FString BrowserUrl = WebBrowser->GetUrl();
	FString Url;
	FString QueryParameters;

	if (BrowserUrl.Split("?", &Url, &QueryParameters))
	{
		if (Url.Equals(CallbackUrl))
		{
			FString ParameterName;
			FString ParameterValue;

			if (QueryParameters.Split("=", &ParameterName, &ParameterValue))
			{
				if (ParameterName.Equals("code"))
				{
					ParameterValue = ParameterValue.Replace(*FString("#"), *FString("")); // remove invalid character

					TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
					RequestObj->SetStringField(ParameterName, ParameterValue);

					FString RequestBody;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

					if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
					{
						TSharedRef<IHttpRequest> ExchangeCodeForTokensRequest = HttpModule->CreateRequest();
						ExchangeCodeForTokensRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnExchangeCodeForTokensResponseRecieved);
						ExchangeCodeForTokensRequest->SetURL(APIUrl + "/exchangecodefortokens");
						ExchangeCodeForTokensRequest->SetVerb("POST");
						ExchangeCodeForTokensRequest->SetHeader("Content-Type", "application/json");
						ExchangeCodeForTokensRequest->SetContentAsString(RequestBody);
						ExchangeCodeForTokensRequest->ProcessRequest();
					}
				}
			}
		}
	}
}

void UMainMenuWidget::SetPlayerLatency()
{
	UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
	if (P4GameInstance)
	{
		PlayerLatency = P4GameInstance->PlayerLatencyTimes.GetTail()->GetValue();
		
		const FString PingString = "Ping: " + FString::FromInt(FMath::RoundToInt(PlayerLatency)) + "ms";
		PingTextBlock->SetText(FText::FromString(PingString));
	}
}

void UMainMenuWidget::OnFindGameButtonClicked()
{
	FindGameButton->SetIsEnabled(false);
	
	FString AccessToken;
	FString MatchmakingTicketId;

	UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
	if (P4GameInstance)
	{
		AccessToken = P4GameInstance->AccessToken;
		MatchmakingTicketId = P4GameInstance->MatchmakingTicketId;
	}

	if (SearchingForGame)
	{
		GetWorld()->GetTimerManager().ClearTimer(PollMatchmakingHandle);
		SearchingForGame = false;

		if (AccessToken.Len() > 0 && MatchmakingTicketId.Len() > 0)
		{
			TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
			RequestObj->SetStringField("ticketId", MatchmakingTicketId);
			
			FString RequestBody;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
			if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
			{
				TSharedRef<IHttpRequest> StopMatchmakingRequest = HttpModule->CreateRequest();
				StopMatchmakingRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnStopMatchmakingResponseRecieved);
				StopMatchmakingRequest->SetURL(APIUrl + "/stopmatchmaking");
				StopMatchmakingRequest->SetVerb("POST");
				StopMatchmakingRequest->SetHeader("Content-Type", "application/json");
				StopMatchmakingRequest->SetHeader("Authorization", AccessToken);
				StopMatchmakingRequest->SetContentAsString(RequestBody);
				StopMatchmakingRequest->ProcessRequest();
			}
			else
			{
				UTextBlock* ButtonTextBlock = (UTextBlock*)FindGameButton->GetChildAt(0);
				ButtonTextBlock->SetText(FText::FromString("Find Game"));

				MatchMakingEventTextBlock->SetText(FText::FromString(""));
				MatchMakingEventTextBlock->SetVisibility(ESlateVisibility::Collapsed);

				FindGameButton->SetIsEnabled(true);
			}
		}
		else
		{
			UTextBlock* ButtonTextBlock = (UTextBlock*)FindGameButton->GetChildAt(0);
			ButtonTextBlock->SetText(FText::FromString("Find Game"));

			MatchMakingEventTextBlock->SetText(FText::FromString(""));
			MatchMakingEventTextBlock->SetVisibility(ESlateVisibility::Collapsed);

			FindGameButton->SetIsEnabled(true);
		}
	}
	else
	{
		if (AccessToken.Len() > 0)
		{
			TSharedRef<FJsonObject> LatencyMapObj = MakeShareable(new FJsonObject);
			LatencyMapObj->SetNumberField(RegionCode, PlayerLatency);

			TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
			RequestObj->SetObjectField("latencyMap", LatencyMapObj);

			FString RequestBody;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
			if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
			{
				TSharedRef<IHttpRequest> StartMatchmakingRequest = HttpModule->CreateRequest();
				StartMatchmakingRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnStartMatchmakingResponseRecieved);
				StartMatchmakingRequest->SetURL(APIUrl + "/startmatchmaking");
				StartMatchmakingRequest->SetVerb("POST");
				StartMatchmakingRequest->SetHeader("Content-Type", "application/json");
				StartMatchmakingRequest->SetHeader("Authorization", AccessToken);
				StartMatchmakingRequest->SetContentAsString(RequestBody);
				StartMatchmakingRequest->ProcessRequest();
			}
			else
			{
				FindGameButton->SetIsEnabled(true);
			}
		}
		else
		{
			FindGameButton->SetIsEnabled(true);
		}
	}
}

void UMainMenuWidget::PollMatchmaking()
{
	FString AccessToken;
	FString MatchmakingTicketId;

	UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
	if (P4GameInstance)
	{
		AccessToken = P4GameInstance->AccessToken;
		MatchmakingTicketId = P4GameInstance->MatchmakingTicketId;
	}

	if (AccessToken.Len() > 0 && MatchmakingTicketId.Len() && SearchingForGame)
	{
		TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
		RequestObj->SetStringField("ticketId", MatchmakingTicketId);

		FString RequestBody;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
		if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer))
		{
			TSharedRef<IHttpRequest> PollmatchmakingRequest = HttpModule->CreateRequest();
			PollmatchmakingRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnPollMatchmakingResponseRecieved);
			PollmatchmakingRequest->SetURL(APIUrl + "/pollmatchmaking");
			PollmatchmakingRequest->SetVerb("POST");
			PollmatchmakingRequest->SetHeader("Content-Type", "application/json");
			PollmatchmakingRequest->SetHeader("Authorization", AccessToken);
			PollmatchmakingRequest->SetContentAsString(RequestBody);
			PollmatchmakingRequest->ProcessRequest();
		}
	}
}

void UMainMenuWidget::OnExchangeCodeForTokensResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("access_token") && JsonObject->HasField("id_token") && JsonObject->HasField("refresh_token"))
			{
				UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
				if (P4GameInstance)
				{
					FString AccessToken = JsonObject->GetStringField("access_token");
					FString IdToken = JsonObject->GetStringField("id_token");
					FString RefreshToken = JsonObject->GetStringField("refresh_token");
					P4GameInstance->SetCognitoTokens(AccessToken, IdToken, RefreshToken);
					
					TSharedRef<IHttpRequest> GetPlayerDataRequest = HttpModule->CreateRequest();
					GetPlayerDataRequest->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnGetPlayerDataResponseRecieved);
					GetPlayerDataRequest->SetURL(APIUrl + "/getplayerdata");
					GetPlayerDataRequest->SetVerb("GET");
					GetPlayerDataRequest->SetHeader("Content-Type", "application/json");
					GetPlayerDataRequest->SetHeader("Authorization", AccessToken);
					GetPlayerDataRequest->ProcessRequest();

					WebBrowser->SetVisibility(ESlateVisibility::Collapsed);

				}
			}
		}
	}
}

void UMainMenuWidget::OnGetPlayerDataResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("playerData"))
			{
				TSharedPtr<FJsonObject> PlayerData = JsonObject->GetObjectField("playerData");
				TSharedPtr<FJsonObject> LevelObject = PlayerData->GetObjectField("Level");
				TSharedPtr<FJsonObject> KillsObject = PlayerData->GetObjectField("Kills");

				FString Level = LevelObject->GetStringField("N");
				FString Kills = KillsObject->GetStringField("N");

				CharacterLevelTextBlock->SetText(FText::FromString(Level));

				WebBrowser->SetVisibility(ESlateVisibility::Collapsed);
				MainMenuVBox->SetVisibility(ESlateVisibility::Visible);

			}
		}

	}

}

void UMainMenuWidget::OnStartMatchmakingResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("ticketId"))
			{
				FString MatchmakingTicketId = JsonObject->GetStringField("ticketId");

				UE_LOG(LogTemp, Warning, TEXT("OnStartMatchmakingResponseRecieved: ticketId=%s"), *MatchmakingTicketId);

				UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
				if (P4GameInstance)
				{
					P4GameInstance->MatchmakingTicketId = MatchmakingTicketId;

					GetWorld()->GetTimerManager().SetTimer(PollMatchmakingHandle, this, &UMainMenuWidget::PollMatchmaking, 1.0f, true, 1.0f);
					// start matchmaking in UI
					SearchingForGame = true;

					UTextBlock* ButtonTextBlock = (UTextBlock*)FindGameButton->GetChildAt(0);
					ButtonTextBlock->SetText(FText::FromString("Cancel Search"));

					MatchMakingEventTextBlock->SetText(FText::FromString("Searching for Game..."));
					MatchMakingEventTextBlock->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
	FindGameButton->SetIsEnabled(true); // out here because if everything fails, we should enable the button so they can try again
}

void UMainMenuWidget::OnStopMatchmakingResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
	if (P4GameInstance)
	{
		P4GameInstance->MatchmakingTicketId = ""; // clear holding tick	et
	}

	UTextBlock* ButtonTextBlock = (UTextBlock*)FindGameButton->GetChildAt(0);
	ButtonTextBlock->SetText(FText::FromString("Find Game"));

	MatchMakingEventTextBlock->SetText(FText::FromString(""));
	MatchMakingEventTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	FindGameButton->SetIsEnabled(true); // out here because if everything fails, we should enable the button so they can try again
}

void UMainMenuWidget::OnPollMatchmakingResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && SearchingForGame)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->HasField("ticket"))
			{
				TSharedPtr<FJsonObject> Ticket = JsonObject->GetObjectField("ticket");
				FString TicketType = Ticket->GetObjectField("Type")->GetStringField("S");

				if (TicketType.Len() > 0)
				{
					GetWorld()->GetTimerManager().ClearTimer(PollMatchmakingHandle);
					SearchingForGame = false;

					// clear matchmaking ticketid
					UProject4GameInstance* P4GameInstance = Cast<UProject4GameInstance>(GetGameInstance());
					if (P4GameInstance)
					{
						P4GameInstance->MatchmakingTicketId = ""; // clear holding tick	et
					}

					if (TicketType.Equals("MatchmakingSucceeded"))
					{
						FindGameButton->SetIsEnabled(false);
						MatchMakingEventTextBlock->SetText(FText::FromString("Found Game! Currently connecting to server..."));

						TSharedPtr<FJsonObject> GameSessionInfo = Ticket->GetObjectField("GameSessionInfo")->GetObjectField("M");
						FString IpAddess = GameSessionInfo->GetObjectField("IpAddress")->GetStringField("S");
						FString Port = GameSessionInfo->GetObjectField("Port")->GetStringField("N");

						TArray<TSharedPtr<FJsonValue>> Players = Ticket->GetObjectField("Players")->GetArrayField("L");
						TSharedPtr<FJsonObject> Player = Players[0]->AsObject()->GetObjectField("M");
						FString PlayerSessionId = Player->GetObjectField("PlayerSessionId")->GetStringField("S");
						FString PlayerId = Player->GetObjectField("PlayerId")->GetStringField("S");

						FString LevelName = IpAddess + ":" + Port;
						const FString& Options = "?PlayerSessionId=" + PlayerSessionId + "?PlayerId=" + PlayerId;

						UE_LOG(LogTemp, Warning, TEXT("options: %s"), *Options);

						// TODO: add onTravelFailed delegate binding to handle failed server connections
						UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), false, Options);
					}
					else {
						UTextBlock* ButtonTextBlock = (UTextBlock*)FindGameButton->GetChildAt(0);
						ButtonTextBlock->SetText(FText::FromString("Join Game"));
						MatchMakingEventTextBlock->SetText(FText::FromString(TicketType + ". Please try again"));
					}
				}
			}
		}
	}
}
