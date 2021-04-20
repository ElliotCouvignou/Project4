// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"
#include "P4SettingsSave.h"
#include "Project4GameInstance.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT4_API UProject4GameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public: 
	UProject4GameInstance();

	virtual void Shutdown() override;

	virtual void Init() override;

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



	// queue-like struct that doesnt require us to empty in order to read
	// Most recent time found at tail
	TDoubleLinkedList<float> PlayerLatencyTimes; 

	UFUNCTION(BlueprintCallable)
		void SetCognitoTokens(FString NewAccessToken, FString NewIdToken, FString NewRefreshToken);

	UFUNCTION(BlueprintCallable)
		void LoadSettingsData();

	UFUNCTION(BlueprintCallable)
		void SaveSettingsData();

	UPROPERTY(BlueprintReadWrite)
		UP4SettingsSave* SettingsSave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities | Pools")
		UP4AbilityPoolGraph* GlobalAbilityPoolGraph;

private:

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
