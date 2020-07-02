// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/GameplayHudWidget.h"
#include "UI/XPBarWidget.h"
#include "UI/ResourceBarsWidget.h"


void UGameplayHudWidget::UpdateCurrentXP(float NewXP)
{
	if (XPBarWidget)
	{
		XPBarWidget->UpdateCurrentXP(NewXP);
	}
}

void UGameplayHudWidget::UpdateMaxXP(float NewMaxXP)
{
	if (XPBarWidget)
	{
		XPBarWidget->UpdateMaxXP(NewMaxXP);
	}
}

void UGameplayHudWidget::UpdatePlayerLevel(float NewLevel)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdatePlayerLevel(NewLevel);
	}
}
