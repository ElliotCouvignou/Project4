// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project4Controller.h"
#include "GameplayHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UGameplayHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:



	UFUNCTION()
		void UpdateCurrentXP(float NewXP);

	UFUNCTION()
		void UpdateMaxXP(float NewMaxXP);

	UFUNCTION()
		void UpdatePlayerLevel(float NewLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NewAbilityErrorMessage(EAbilityErrorText ErrorType);

protected:
	// gets created and filled out by BP
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UXPBarWidget* XPBarWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UResourceBarsWidget* ResourceBarsWidget;
};
