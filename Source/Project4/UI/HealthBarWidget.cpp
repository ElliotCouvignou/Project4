// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

UHealthBarWidget::UHealthBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();	
	
}


void UHealthBarWidget::UpdateHealthPercent(float percent)
{
	if (HealthBar) 
	{
		HealthBar->SetPercent(percent);
	}
}

void UHealthBarWidget::Hide()
{
	HealthBar->SetVisibility(ESlateVisibility::Visible);
}

void UHealthBarWidget::Show()
{
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
}