// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingStatusBarWidget.generated.h"

/**
 *  floating widget above characters to display health and other
 *  soon to be added features
 */
UCLASS()
class PROJECT4_API UFloatingStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCharacterName(const FText& NewName);

	UPROPERTY(BlueprintReadWrite)
		AActor* Owner;
};
