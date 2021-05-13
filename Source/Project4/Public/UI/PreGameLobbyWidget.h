// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreGameLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UPreGameLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InitPlayerReadyStatus(const TArray<bool>& States);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayerReadyStatusChanged(int index, bool NewState);
};
