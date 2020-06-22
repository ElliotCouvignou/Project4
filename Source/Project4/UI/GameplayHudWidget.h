// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XPBarWidget.h"
#include "GameplayHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UGameplayHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UXPBarWidget* XPBarWidget;

	UFUNCTION()
		void UpdateCurrentXP(float NewXP);

	UFUNCTION()
		void UpdateMaxXP(float NewMaxXP);


	
};
