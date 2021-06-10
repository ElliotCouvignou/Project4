// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "AbilitySystem/P4AbilityPoolsDataAsset.h"
#include "P4SettingsSave.h"
#include "P4CurrentGameSave.h"
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

	/****************************************/
	/*         Online Steam Subsystem       */
	/****************************************/

	UPROPERTY(BlueprintReadOnly)
		FString Url;


	// Tokens expire every hour, this timer should request new tokens every 59 mins to 'refresh'
	UPROPERTY(BlueprintReadOnly)
		FTimerHandle RetrieveNewTokensHandle;

	UPROPERTY(BlueprintReadOnly)
		FTimerHandle GetLatencyTimeLandleHandle;

	/* Info Struct saved by host to be read from again once new level finished loading */
	//UPROPERTY(BlueprintReadOnly)
	//	FPreGameLobbyInfoStruct PreGameLobbyInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
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
		void CreateNewCurrentGameData();

	UFUNCTION(BlueprintCallable)
		void LoadCurrentGameData();

	UFUNCTION(BlueprintCallable)
		void SaveCurrentGameData();

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

		/* called by host to servertravel */
	UFUNCTION(BlueprintCallable)
		void HostTravelToNextLevel(FString TravelUrl);

	/* called by host to clienttravel */
	UFUNCTION(BlueprintCallable)
		void ClientTravelToNextLevel();

	UFUNCTION(BlueprintCallable)
		void OpenFriendInvites();


	UPROPERTY(BlueprintReadWrite)
		UP4SettingsSave* SettingsSave;

	UPROPERTY(BlueprintReadWrite)
		UP4CurrentGameSave* CurrentGameSave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities | Pools")
		UP4AbilityPoolsDataAsset* GlobalAbilityDataAsset;

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
	void OnSessionUserInviteAccepted(bool bWasSuccessful, int32 ControllerIndex, TSharedPtr<const FUniqueNetId> NetId, const FOnlineSessionSearchResult& SessionToJoin);
	void OnJoinInviteSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FOnJoinSessionCompleteDelegate OnJoinInviteSessionCompleteDelegate;


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

	UPROPERTY()
		FString CurrentGameSaveSlot = "CurrentGameSaveSlot";

	UFUNCTION()
		void RetrieveNewTokens();

	UFUNCTION()
		void GetLatencyTime();

	void OnRetrieveNewTokensCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetLatencyTimeResponseRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
