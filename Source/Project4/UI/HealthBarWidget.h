// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UHealthBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateHealthPercent(float percent);

	void Hide();

	void Show();

	// instead of manually creating a progress bar, find one and implement there
	// allows furhter customization within editor instead of code only, meta tag does this
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ACharacter> player;
};
