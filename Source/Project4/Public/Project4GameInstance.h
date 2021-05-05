// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"
#include "P4SettingsSave.h"
#include "P4PreGameLobbyGameMode.h"

#include "Interfaces/SessionMenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h" 
#include "OnlineSubsystem.h"

#include "UI/MainMenuWidget.h"
#include "Project4GameInstance.generated.h"




/**
 * 
 */
UCLASS()
class PROJECT4_API UProject4GameInstance : public UGameInstance, public ISessionMenuInterface
{
	GENERATED_BODY()
	

public: 
	UProject4GameInstance();

	virtual void Shutdown() override;

	virtual void Init() override;

	/******************************/
	/*          AWS Things        */
	/******************************/

	UPROPERTY(BlueprintReadOnly)
		FString AccessToken;

	UPROPERTY(BlueprintReadOnly)
		FString IdToken;

	UPROPERTY(BlueprintReadOnly)
		FString RefreshToken;

	UPROPERTY(BlueprintReadOnly)
		FString MatchmakingTicketId;

	// Tokens expire every hour, this timer should request new tokens every 59 mins to 'refresh'
	UPROPERTY(BlueprintReadOnly)
		FTimerHandle RetrieveNewTokensHandle;

	UPROPERTY(BlueprintReadOnly)
		FTimerHandle GetLatencyTimeLandleHandle;

	/* Info Struct saved by host to be read from again once new level finished loading */
	//UPROPERTY(BlueprintReadOnly)
	//	FPreGameLobbyInfoStruct PreGameLobbyInfo;

	UPROPERTY(BlueprintReadOnly)
		bool LoadFromPreGameLobbyInfo = false;



	// queue-like struct that doesnt require us to empty in order to read
	// Most recent time found at tail
	TDoubleLinkedList<float> PlayerLatencyTimes; 

	UFUNCTION(BlueprintCallable)
		void SetCognitoTokens(FString NewAccessToken, FString NewIdToken, FString NewRefreshToken);

	UFUNCTION(BlueprintCallable)
		void LoadSettingsData();

	UFUNCTION(BlueprintCallable)
		void SaveSettingsData();

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();


	/******************************/
	/*      Online Subsystem      */
	/******************************/

	///// ISessionMenuInterface /////////////////// 
	UFUNCTION(BlueprintCallable)
		void Host(FString ServerName) override;

	UFUNCTION(BlueprintCallable)
		void JoinSession(const int Index) override;

	UFUNCTION(BlueprintCallable)
		void EndSession() override;

	UFUNCTION(BlueprintCallable)
		void OpenSessionListMenu() override;
	///// ISessionMenuInterface /////////////////// 



	UPROPERTY(BlueprintReadWrite)
		UP4SettingsSave* SettingsSave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities | Pools")
		UP4AbilityPoolGraph* GlobalAbilityPoolGraph;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class UMainMenuWidget> MenuClass;
	class UMainMenuWidget* MainMenu;

private:

	/******************************/
	/*      Online Subsystem      */
	/******************************/


	FString DesiredServerName;
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;


	/******************************/
	/*          AWS Things        */
	/******************************/

	FHttpModule* HttpModule;

	UPROPERTY()
		FString APIUrl;

	UPROPERTY()
		FString RegionCode;

	UPROPERTY()
		FString SettingsSaveSlot = "SettingsSlaveSlot";

	UFUNCTION()
		void RetrieveNewTokens();

	UFUNCTION()
		void GetLatencyTime();

	void OnRetrieveNewTokensCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetLatencyTimeResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
