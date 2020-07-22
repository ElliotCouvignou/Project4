// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#include "Project4Controller.h"
#include "Project4GameMode.h"
#include "Characters/Project4Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "UI/FloatingTextWidgetComponent.h"
#include "UI/GameplayHudWidget.h"
#include "UI/BuffIconsWidget.h"


AProject4Controller::AProject4Controller(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

// called in onRep_PC in playercharacterbase
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

TSubclassOf< AP4PlayerCharacterBase> AProject4Controller::GetPlayerCharacterClass()
{
	return PlayerCharacterClass;
}

void AProject4Controller::SetPlayerCharacterClass(TSubclassOf<AP4PlayerCharacterBase> CharacterClass)
{
	PlayerCharacterClass = CharacterClass;
}

void AProject4Controller::ClientRequestRespawn_Implementation()
{
	AProject4GameMode* GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->RespawnPlayer(this);
	}
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

void AProject4Controller::SetUIRespawnCountdown_Implementation(float Duration)
{
	GameplayHUDWidget->SetRespawnCountdown(Duration);
}

void AProject4Controller::SendUIAbilityError(EAbilityErrorText ErrorType)
{
	GameplayHUDWidget->NewAbilityErrorMessage(ErrorType);
}

void AProject4Controller::SetupUIAbilityToHotBarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability)
{
	GameplayHUDWidget->SetAbilityHotbarBlock(BlockIndex, Ability);
}

void AProject4Controller::SendBuffIconToUI_Implementation(const FGameplayTag& BuffTag, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle)
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->SendBuffIconToUI(BuffTag, SpecApplied, ActiveHandle);
	}
}

void AProject4Controller::UpdateBuffIconStacksUI_Implementation(const FGameplayTag& BuffTag, int32 Count)
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->UpdateBuffIconStacksUI(BuffTag, Count);
	}
}

void AProject4Controller::RemoveBuffIconFromUI_Implementation(const FGameplayTag& BuffTag)
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->RemoveBuffIconFromUI(BuffTag);
	}
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


