// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#include "Project4Controller.h"
#include "Characters/Project4Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "UI/FloatingTextWidgetComponent.h"
#include "UI/GameplayHudWidget.h"


AProject4Controller::AProject4Controller(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AProject4Controller::CreateMainHUDWidget()
{

	if (GameplayHUDWidget || !GameplayHUDWidgetClass || !IsLocalPlayerController())
	{
		return;
	}

	GameplayHUDWidget = CreateWidget<UGameplayHudWidget>(this, GameplayHUDWidgetClass);
	GameplayHUDWidget->AddToViewport();
}

UGameplayHudWidget* AProject4Controller::GetMainHUDWidget()
{
	return GameplayHUDWidget;
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

void AProject4Controller::UpdateUICurrentXP_Implementation(float NewXP)
{
	GameplayHUDWidget->UpdateCurrentXP(NewXP);
}

void AProject4Controller::UpdateUIMaxXP_Implementation(float NewMaxXP)
{
	GameplayHUDWidget->UpdateMaxXP(NewMaxXP);
}

void AProject4Controller::UpdateUILevel_Implementation(float NewLevel)
{
	GameplayHUDWidget->UpdatePlayerLevel(NewLevel);
}

void AProject4Controller::SendUIAbilityError(EAbilityErrorText ErrorType)
{
	GameplayHUDWidget->NewAbilityErrorMessage(ErrorType);
}


// Server only
void AProject4Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AProject4Controller::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	//CreateMainHUDWidget();
}


