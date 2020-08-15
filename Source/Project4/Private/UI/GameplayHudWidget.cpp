// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/GameplayHudWidget.h"
#include "UI/XPBarWidget.h"
#include "UI/ResourceBarsWidget.h"
#include "UI/AbilityHotbar.h"
#include "UI/BuffIconsWidget.h"
#include "UI/CrosshairWidget.h"
#include "UI/CharacterInfoWidget.h"


void UGameplayHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind Delegate for when Screen size changes
	GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &UGameplayHudWidget::OnViewportResizedCallback);
}


void UGameplayHudWidget::OnViewportResizedCallback(FViewport* ActiveViewport, uint32 UnknownUnsignedInt)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwningPlayer());
	if (PC)
	{
		PC->FindCrosshairOffsetPitchAngle(ActiveViewport->GetSizeXY(), CrosshairScreenYOffset);
	}
}

void UGameplayHudWidget::UpdatePlayerName(const FString& NewName)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdatePlayerName(NewName);
	}
}
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
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateHealthMax(NewValue);
	}
}

void UGameplayHudWidget::UpdateHealthRegen(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateHealthRegen(NewValue);
	}
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateHealthRegen(NewValue);
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
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateManaMax(NewValue);
	}
}

void UGameplayHudWidget::UpdateManaRegen(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateManaRegen(NewValue);
	}
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateManaMax(NewValue);
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
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateEnduranceMax(NewValue);
	}
}

void UGameplayHudWidget::UpdateEnduranceRegen(float NewValue)
{
	if (ResourceBarsWidget)
	{
		ResourceBarsWidget->UpdateEnduranceRegen(NewValue);
	}
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateEnduranceRegen(NewValue);
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
	if(CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdatePlayerLevel(NewLevel);
	}
}

void UGameplayHudWidget::UpdateArmor(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateArmor(NewValue);
	}
}

void UGameplayHudWidget::UpdateMagicResistance(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateMagicResistance(NewValue);
	}
}

void UGameplayHudWidget::UpdateMovementSpeed(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateMovementSpeed(NewValue);
	}
}

void UGameplayHudWidget::UpdateAttackPower(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateAttackPower(NewValue);
	}
}

void UGameplayHudWidget::UpdateMagicPower(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateMagicPower(NewValue);
	}
}

void UGameplayHudWidget::UpdateAttackSpeed(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateAttackSpeed(NewValue);
	}
}

void UGameplayHudWidget::UpdateCritChance(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateCritChance(NewValue);
	}
}

void UGameplayHudWidget::UpdateCritDamage(float NewValue)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateCritDamage(NewValue);
	}
}

void UGameplayHudWidget::SetAbilityHotbarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability)
{
	if (AbilityHotbar)
	{
		AbilityHotbar->SetupHotbarAbility(BlockIndex, Ability);
	}
}

void UGameplayHudWidget::GetCrosshairGroundLocation(float MaxSearchDistance, FVector& EndLocation, FVector& StartLocation)
{
	if (CrosshairWidget && GetOwningPlayer()->IsLocalController())
	{
		CrosshairWidget->FindGroundLocation(MaxSearchDistance, EndLocation, StartLocation);
	}
}


