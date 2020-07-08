// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/GameplayHudWidget.h"
#include "UI/XPBarWidget.h"
#include "UI/ResourceBarsWidget.h"


void UGameplayHudWidget::UpdateHealth(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateHealth(NewValue);
	}
}
void UGameplayHudWidget::UpdateHealthMax(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateHealthMax(NewValue);
	}
}

void UGameplayHudWidget::UpdateHealthRegen(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateHealthRegen(NewValue);
	}
}

void UGameplayHudWidget::UpdateMana(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateMana(NewValue);
	}
}
void UGameplayHudWidget::UpdateManaMax(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateManaMax(NewValue);
	}
}

void UGameplayHudWidget::UpdateManaRegen(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateManaRegen(NewValue);
	}
}

void UGameplayHudWidget::UpdateEndurance(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateEndurance(NewValue);
	}
}
void UGameplayHudWidget::UpdateEnduranceMax(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateEnduranceMax(NewValue);
	}
}

void UGameplayHudWidget::UpdateEnduranceRegen(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateEnduranceRegen(NewValue);
	}
}

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
