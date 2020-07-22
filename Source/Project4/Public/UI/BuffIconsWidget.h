// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "BuffIconsWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UBuffIconsWidget : public UUserWidget
{
	GENERATED_BODY()


public: 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NewBuffIconWithTimer(UTexture2D* IconImageTexture, float BuffDuration, const FText& BuffToolTipText, const FGameplayTag& BuffTag, bool BuffResetTimer, bool Stackable);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateBuffStackCount(const FGameplayTag& BuffTag, int32 Count);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void RemoveBuffIconFromUI(const FGameplayTag& BuffTag);
};
