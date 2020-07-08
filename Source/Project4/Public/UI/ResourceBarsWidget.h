// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceBarsWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UResourceBarsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdatePlayerLevel(float NewLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateHealth(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateHealthMax(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateHealthRegen(float NewValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMana(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateManaMax(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateManaRegen(float NewValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateEndurance(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateEnduranceMax(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateEnduranceRegen(float NewValue);
};
