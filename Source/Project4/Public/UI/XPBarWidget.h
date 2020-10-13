// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UXPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateCurrentXP(float NewXP);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMaxXP(float NewMaxXP);

	UFUNCTION()
		void SetStartingXP(float value) { StartingXP = value; }

protected: 

	// Xp to level on last level, since xp isnt reset on level we must track the xp 
	// threshold of last levelup
	UPROPERTY(BlueprintReadWrite)
		float StartingXP;
};
