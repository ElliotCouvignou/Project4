// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GameplayHudWidget.h"


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
