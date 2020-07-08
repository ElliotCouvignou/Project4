// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/WidgetComponent.h"

#include "GameplayHUD.generated.h"

/**
 *  This is pretty unused atm might even delete
 */
UCLASS()
class PROJECT4_API AGameplayHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AGameplayHUD();

	virtual void DrawHUD() override; 

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};
