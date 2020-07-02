// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#include "Project4Controller.h"
#include "Project4Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "UI/FloatingTextWidgetComponent.h"
#include "UI/GameplayHudWidget.h"



AProject4Controller::AProject4Controller(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AProject4Controller::SetMainHUDWidget(UGameplayHudWidget* NewWidget)
{
	GameplayHUDWidget = NewWidget;
}

void AProject4Controller::DisplayDamageNumber_Implementation(float DamageValue, AProject4Character* TargetCharacter)
{
	UFloatingTextWidgetComponent* DamageText = NewObject<UFloatingTextWidgetComponent>(TargetCharacter, FloatingTextClass);
	DamageText->RegisterComponent();
	DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageText->SetMixedDamageText(DamageValue);
}

void AProject4Controller::DisplayHealNumber_Implementation(float HealValue, AProject4Character* TargetCharacter)
{
	UFloatingTextWidgetComponent* DamageText = NewObject<UFloatingTextWidgetComponent>(TargetCharacter, FloatingTextClass);
	DamageText->RegisterComponent();
	DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageText->SetHealText(HealValue);
}

void AProject4Controller::UpdateCurrentXPUI_Implementation(float NewXP)
{
	GameplayHUDWidget->UpdateCurrentXP(NewXP);
}

void AProject4Controller::UpdateMaxXPUI_Implementation(float NewMaxXP)
{
	GameplayHUDWidget->UpdateMaxXP(NewMaxXP);
}

void AProject4Controller::UpdateLevelUI_Implementation(float NewLevel)
{
	GameplayHUDWidget->UpdatePlayerLevel(NewLevel);
}

void AProject4Controller::SendAbilityErrorUI(EAbilityErrorText ErrorType)
{
	GameplayHUDWidget->NewAbilityErrorMessage(ErrorType);
}



