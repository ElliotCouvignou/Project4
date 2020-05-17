// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayHUD.h"



AGameplayHUD::AGameplayHUD() : Super()
{

}

void AGameplayHUD::DrawHUD()
{
	Super::DrawHUD();
}


void AGameplayHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarClass)
	{
		HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarClass);
		if (HealthBarWidget)
			HealthBarWidget->AddToViewport();
	}
}

void AGameplayHUD::UpdateHealthBar(float percent)
{
	if (HealthBarWidget) {
		HealthBarWidget->UpdateHealthPercent(percent);
	}
}
