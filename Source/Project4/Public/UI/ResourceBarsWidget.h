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

	/* Called when attribute set gets reinitialized, used just to ensure things dont get left behind */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ReInitValues();

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
