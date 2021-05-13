// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#include "Project4Controller.h"
#include "Project4GameMode.h"
#include "Characters/P4PlayerCharacterBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "P4PreGameLobbyGameMode.h"
#include "Project4GameInstance.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/P4FloatingTextWidget.h"
#include "UI/PreGameLobbyWidget.h"
#include "UI/GameplayHudWidget.h"
#include "UI/BuffIconsWidget.h"
#include "..\Public\Project4Controller.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AProject4Controller::AProject4Controller(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AProject4Controller::ServerChooseCharacter_Implementation(const EClassAbilityPoolType& PoolType)
{
	if (PoolType == EClassAbilityPoolType::None)
		return;

	AP4PreGameLobbyGameMode* GM = Cast<AP4PreGameLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnPlayerCharacterSelected(this, PoolType);
		//OnCharacterSelected(PoolType);
	}
	
	//AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());
	//if (PChar)
	//{
	//	PChar->OnCharacterSelected(PoolType);
	//}		
}

void AProject4Controller::OnCharacterSelected_Implementation(const EClassAbilityPoolType& PoolType)
{
	print(FString("OnCharacterSelected_Implementation"));
}


void AProject4Controller::FindCrosshairOffsetPitchAngle(const FIntPoint& ViewportSizeXY, float CrosshairScreenYOffset)
{
	// All math here but essentialy trying to find distance between screen's center and crosshair
	// in world space, this is necessary so we can use skillshots aiming at our displaced crosshair 
	float CenterScreenX = ViewportSizeXY.X / 2;
	float CenterScreenY = ViewportSizeXY.Y / 2;
	float CenterCrosshairY = CenterScreenY + (ViewportSizeXY.Y / 1024.f * CrosshairScreenYOffset);

	//CrosshairScreenLocation.X = CenterScreenX;
	//CrosshairScreenLocation.Y = CenterCrosshairY;
	// Convert Screen location to world space
	FVector ScreenCenterDirection;	FVector CrosshairDirection;	FVector Useless;
	DeprojectScreenPositionToWorld(CenterScreenX, CenterScreenY, Useless, ScreenCenterDirection);
	DeprojectScreenPositionToWorld(CenterScreenX, CenterCrosshairY, Useless, CrosshairDirection);
	
	CrosshairOffsetPitchAngle = FMath::Acos(FVector::DotProduct(ScreenCenterDirection, CrosshairDirection));
	CrosshairOffsetPitchAngle = FMath::RadiansToDegrees(CrosshairOffsetPitchAngle);

	SendCrosshairOffsetAngleToServer(CrosshairOffsetPitchAngle);
}

void AProject4Controller::ClearWidgets_Implementation()
{
	// TODO: replace with call to fade or smth
	if (PreGameLobbyWidget)
	{
		PreGameLobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
		//PreGameLobbyWidget->RemoveFromViewport();
	}
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
		//GameplayHUDWidget->RemoveFromViewport();
	}
	bShowMouseCursor = false;
}


void AProject4Controller::CreateMainHUDWidget_Implementation()
{
	if (!GameplayHUDWidgetClass || !IsLocalPlayerController())
	{
		return;
	}
	// remove pregamelobby widget if visible 
	if (PreGameLobbyWidget)
	{
		PreGameLobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
		PreGameLobbyWidget->RemoveFromViewport();
	}

	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
		GameplayHUDWidget->RemoveFromViewport();
		GameplayHUDWidget = nullptr;
	}

	print(FString("Create New MAin HUD Widget !!!!!!!!!!!\n\n"));
	GameplayHUDWidget = CreateWidget<UGameplayHudWidget>(this, GameplayHUDWidgetClass);
	GameplayHUDWidget->AddToViewport();
}



void AProject4Controller::CreatePreGameLobbyWidget_Implementation()
{
	if (!PreGameLobbyWidgetClass || !IsLocalPlayerController())
	{
		return;
	}

	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
		GameplayHUDWidget->RemoveFromViewport();
	}

	if (PreGameLobbyWidget)
	{
		PreGameLobbyWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PreGameLobbyWidget = CreateWidget<UPreGameLobbyWidget>(this, PreGameLobbyWidgetClass);
		
	}

	PreGameLobbyWidget->AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, PreGameLobbyWidget, EMouseLockMode::DoNotLock);
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	print(FString("PregameLobbyWidget, sgow cursor"));
}

void AProject4Controller::PossessCamera_Implementation(const ACameraActor* Camera)
{
	if (Camera)
	{
		SetViewTarget((AActor*)Camera);
	}
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

void AProject4Controller::GetPlayerViewPointBP(FVector& out_Location, FRotator& out_Rotation)
{
	GetPlayerViewPoint(out_Location, out_Rotation);
}

void AProject4Controller::ServerPlayerReadyStatusChanged_Implementation()
{
	AP4PreGameLobbyGameMode* GM = Cast<AP4PreGameLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnPlayerReadyStatusChanged(this);
	}
}



void AProject4Controller::ClientRequestRespawn_Implementation()
{
	AProject4GameMode* GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->RespawnPlayer(this);
	}
}

void AProject4Controller::SendCrosshairOffsetAngleToServer_Implementation(float NewAngle)
{
	CrosshairOffsetPitchAngle = NewAngle;
}


void AProject4Controller::DisplayDamageNumber_Implementation(FP4DamageNumber Damage, AProject4Character* TargetCharacter)
{
	UP4FloatingTextWidget* DamageText = TargetCharacter->GetFloatingTextWidget();
	if (DamageText)
	{
		DamageText->SetDamageText(Damage);
	}
}

void AProject4Controller::DisplayHealNumber_Implementation(FP4DamageNumber Heal, AProject4Character* TargetCharacter)
{
	UP4FloatingTextWidget* DamageText = TargetCharacter->GetFloatingTextWidget();
	if (DamageText)
	{
		DamageText->SetHealText(Heal);
	}
}

void AProject4Controller::SetUIRespawnCountdown_Implementation(float Duration)
{
	GameplayHUDWidget->SetRespawnCountdown(Duration);
}

void AProject4Controller::ClientDisplayWidgetToViewport_Implementation(TSubclassOf<UUserWidget> WidgetClass)
{
	if (WidgetClass)
	{
		UUserWidget* widget = CreateWidget(this, WidgetClass);
		widget->AddToViewport();
	}
}

void AProject4Controller::Client_DisplaySkillDropGeneralPoolWidget_Implementation(const TArray<EAbilityCategory>& Pools)
{
	GameplayHUDWidget->SetupGeneralSkillPoolWidget(Pools);
}

void AProject4Controller::Client_DisplaypAbilityChoicesWidget_Implementation(const TArray<TSubclassOf<UP4GameplayAbility>>& Abilities)
{
	GameplayHUDWidget->SetupAbilitySelectionWidget(Abilities);
}

void AProject4Controller::Client_DisplaypAbilityModifierAbilityChoicesWidget_Implementation(const TArray<TSubclassOf<UP4GameplayAbility>>& AbilityChoices)
{
	GameplayHUDWidget->SetupAbilityModifierAbilitySelectionWidget(AbilityChoices);
}

void AProject4Controller::Client_DisplaypAbilityModifierChoicesWidget_Implementation(const TArray<UP4AbilityModifierInfo*>& AbilityModifierInfos)
{
	GameplayHUDWidget->SetupAbilityModifierSelectionWidget(AbilityModifierInfos);
}

void AProject4Controller::Client_PlayerReadyStatusChanged_Implementation(int PlayerIndex, bool NewVal)
{
	PlayerReadtStatusChanged(PlayerIndex, NewVal);
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

void AProject4Controller::OpenCharacterInfoMenu()
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->OpenCharacterInfoWidget();
	}
}

void AProject4Controller::OpenInventoryBagMenu()
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->OpenInventoryBagWidget();
	}
}

void AProject4Controller::OpenSkillsMenu()
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->OpenSkillsWidget();
	}
}

void AProject4Controller::OpenAbilityMenu()
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->OpenAbilityMenuWidget();
	}
}

void AProject4Controller::TryOpenEscapeMenu()
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->TryOpenEscapeMenu();
	}
}

void AProject4Controller::BeginDestroy()
{
	Super::BeginDestroy();

	print(FString("AProject4Controller::BeginDestroy()"));

	// remove pregamelobby widget if visible 
	if (PreGameLobbyWidget)
	{
		print(FString("\n\nRemove PregameLobyy\n\n "));
		PreGameLobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
		PreGameLobbyWidget->RemoveFromViewport();
	}
}

void AProject4Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OpenCharacterInfoMenu", IE_Pressed, this, &AProject4Controller::OpenCharacterInfoMenu);
	InputComponent->BindAction("OpenInventoryBagMenu", IE_Pressed, this, &AProject4Controller::OpenInventoryBagMenu);
	InputComponent->BindAction("OpenSkillsMenu", IE_Pressed, this, &AProject4Controller::OpenSkillsMenu);
	InputComponent->BindAction("OpenAbilityMenu", IE_Pressed, this, &AProject4Controller::OpenAbilityMenu);
	InputComponent->BindAction("TryOpenEscapeMenu", IE_Pressed, this, &AProject4Controller::TryOpenEscapeMenu);

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


