// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "MainMenuWidget.generated.h"

class UWebBrowser;
class UButton;
class UTextBlock;
class UVerticalBox;

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int CurrentPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int MaxPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PROJECT4_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
		FTimerHandle SetPlayerLatencyHandle;

	// TODO: This should be replaced with websocket api's so i dont rely on continuous polling but too bad im stupid and dont know websockets :/ 
	UPROPERTY()
		FTimerHandle PollMatchmakingHandle;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InitializeSessionsList(const TArray<FServerData>& ServerData);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	FHttpModule* HttpModule;

	UPROPERTY(BlueprintReadOnly)
		FString LoginUrl;

	UPROPERTY(BlueprintReadOnly)
		FString APIUrl;

	UPROPERTY(BlueprintReadOnly)
		FString CallbackUrl;

	UPROPERTY(BlueprintReadOnly)
		FString RegionCode;

	UPROPERTY(BlueprintReadOnly)
		UWebBrowser* WebBrowser;


	// main menu to show after user login
	UPROPERTY()
		UVerticalBox* MainMenuVBox;

	UPROPERTY()
		UButton* FindGameButton;

	UPROPERTY()
		UTextBlock* CharacterNameTextBlock;

	UPROPERTY()
		UTextBlock* CharacterLevelTextBlock;

	UPROPERTY()
		UTextBlock* PingTextBlock;

	UPROPERTY()
		UTextBlock* MatchMakingEventTextBlock;
	
	UPROPERTY()
		float PlayerLatency;

	UPROPERTY()
		bool SearchingForGame;

	UFUNCTION()
		void HandleLoginUrlChange();

	UFUNCTION()
		void SetPlayerLatency();

	UFUNCTION()
		void OnFindGameButtonClicked();

	UFUNCTION()
		void PollMatchmaking();

	void OnExchangeCodeForTokensResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetPlayerDataResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnStartMatchmakingResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnStopMatchmakingResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnPollMatchmakingResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
