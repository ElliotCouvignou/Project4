// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Project4GameInstance.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "TextReaderComponent.h"


UProject4GameInstance::UProject4GameInstance()
{
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));

	APIUrl = TextReader->ReadFile("Urls/APIUrl.txt");
	RegionCode = TextReader->ReadFile("Urls/RegionCode.txt");
	HttpModule = &FHttpModule::Get();
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
	GetWorld()->GetTimerManager().SetTimer(GetLatencyTimeLandleHandle, this, &UProject4GameInstance::GetLatencyTime, 1.f, true, 0.f);
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
