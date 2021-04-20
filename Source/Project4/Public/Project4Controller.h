// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "Project4.h"
#include "Project4Controller.generated.h"

/* UNUSED ATM */
UENUM(BlueprintType)
enum class EDamageTypes : uint8
{
	Physical			UMETA(DisplayName = "Physical"),
	Magic				UMETA(DisplayName = "Magic"),
	Nature				UMETA(DispalyName = "Naute")
	// TODO: Add elemental damage types here
};

/* Damage Nubmer class to display */
USTRUCT(BlueprintType)
struct PROJECT4_API FP4DamageNumber
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DamageAmount;

	/* contains Tags for info about Crit, Damage types, etc. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGameplayTagContainer Tags;

	FP4DamageNumber() {}

	FP4DamageNumber(float InDamageAmount, FGameplayTagContainer InTags) : DamageAmount(InDamageAmount)
	{
		// Copy tag container
		Tags.AppendTags(InTags);
	}
};

// location of this struct might need to be moved if necessary
// ONLY CREATE A NEW ENUM IF: you want to bind an enum to a NEW inputAction event DisplayName
UENUM(BlueprintType)
enum class EAbilityErrorText : uint8
{
	OutOfRange			UMETA(DisplayName = "OutOfRange"),
	OutOfMana			UMETA(DisplayName = "OutOfMana"),
	NoTarget			UMETA(DisplayName = "NoTarget"),
	InvalidTarget       UMETA(DisplayName = "InvalidTarget"),
	InvalidState		UMETA(DisplayName = "InvalidState"),
	OnCooldown			UMETA(DisplayName = "OnCooldown")
};

/**
 * 
 */
UCLASS()
class PROJECT4_API AProject4Controller : public APlayerController
{
	GENERATED_BODY()

	/***************************/
	/*       Components        */
	/***************************/


public:
	AProject4Controller(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/* Pitch offset from screen center due to crosshair Y offset. (Rads) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Crosshair")
		float CrosshairOffsetPitchAngle;

	/* Called on Gameplayhud creation (in BP) and when screensize changes
	   Results are saved in CrosshaitCenterX/CrosshairCenterY */
	UFUNCTION(BlueprintCallable)
		void FindCrosshairOffsetPitchAngle(const FIntPoint& ViewportSizeXY, float CrosshairScreenYOffset);

	// creates a widget based on gameplayHUDwidgetclass
		void CreateMainHUDWidget();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		class UGameplayHudWidget* GetMainHUDWidget();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		TSubclassOf<AP4PlayerCharacterBase> GetPlayerCharacterClass();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void SetPlayerCharacterClass(TSubclassOf<AP4PlayerCharacterBase> CharacterClass);

	/* Calls Parent GetPlayerViewPoint, This is just exposing to BP */
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetPlayerViewPointBP(FVector& out_Location, FRotator& out_Rotation);

	/************************************/
	/* Server RPC/Functions From Client */
	/************************************/

	// Called after respawn button pressed
	// need server for gamemode access to respawn
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ClientRequestRespawn();
	void ClientRequestRespawn_Implementation();
	bool ClientRequestRespawn_Validate() { return true; }

	/* Called on Crosshair creation and viewport/resolution size changed */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void SendCrosshairOffsetAngleToServer(float NewAngle);
	void SendCrosshairOffsetAngleToServer_Implementation(float NewAngle);
	bool SendCrosshairOffsetAngleToServer_Validate(float NewAngle) { return true; }

	/*******************************/
	/* Client RPC/Functions for UI */
	/*******************************/

	// Server calls this fucntion to display damage numbers to a player
	UFUNCTION(Client, Reliable, WithValidation)
		void DisplayDamageNumber(FP4DamageNumber Damage, AProject4Character* TargetCharacter);
	void DisplayDamageNumber_Implementation(FP4DamageNumber Damage, AProject4Character* TargetCharacter);
	bool DisplayDamageNumber_Validate(FP4DamageNumber Damage, AProject4Character* TargetCharacter) { return true; }

	// Server calls this fucntion to display damage numbers to a player
	UFUNCTION(Client, Reliable, WithValidation)
		void DisplayHealNumber(float HealValue, AProject4Character* TargetCharacter);
	void DisplayHealNumber_Implementation(float HealValue, AProject4Character* TargetCharacter);
	bool DisplayHealNumber_Validate(float HealValue, AProject4Character* TargetCharacter) { return true; }

	UFUNCTION(Client, Reliable, WithValidation)
		void SetUIRespawnCountdown(float Duration);
	void SetUIRespawnCountdown_Implementation(float Duration);
	bool SetUIRespawnCountdown_Validate(float Duration) { return true; }

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Skill Drop")
		void ClientDisplayWidgetToViewport(TSubclassOf<UUserWidget> WidgetClass);
	void ClientDisplayWidgetToViewport_Implementation(TSubclassOf<UUserWidget> WidgetClass);
	bool ClientDisplayWidgetToViewport_Validate(TSubclassOf<UUserWidget> WidgetClass) { return true; }

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
		void Client_DisplaySkillDropGeneralPoolWidget(const TArray<EAbilityCategory>& Pools);
	void Client_DisplaySkillDropGeneralPoolWidget_Implementation(const TArray<EAbilityCategory>& Pools);
	bool Client_DisplaySkillDropGeneralPoolWidget_Validate(const TArray<EAbilityCategory>& Pools) { return true; }

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
		void Client_DisplaypAbilityChoicesWidget(const TArray<TSubclassOf<UP4GameplayAbility>>& Abilities);
	void Client_DisplaypAbilityChoicesWidget_Implementation(const TArray < TSubclassOf<UP4GameplayAbility>>& Abilities);
	bool Client_DisplaypAbilityChoicesWidget_Validate(const TArray < TSubclassOf<UP4GameplayAbility>>& Abilities) { return true; }


	// Called on Ability Error (e.g. Out of range, Out of mana, etc.)
	// doesn't need client RPC since ability error texts occurs during
	// ability try activate which is done by owning clients too
	UFUNCTION(BlueprintCallable)
		void SendUIAbilityError(EAbilityErrorText ErrorType);

	UFUNCTION(BlueprintCallable)
		void SetupUIAbilityToHotBarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);


	/*******************/
	/*    Buff Icons   */
	/*******************/

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
		void SendBuffIconToUI(const FGameplayTag& BuffTag, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);
	void SendBuffIconToUI_Implementation(const FGameplayTag& BuffTag, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);
	bool SendBuffIconToUI_Validate(const FGameplayTag& BuffTag, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle) { return true; }

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
		void UpdateBuffIconStacksUI(const FGameplayTag& BuffTag, int32 Count);
	void UpdateBuffIconStacksUI_Implementation(const FGameplayTag& BuffTag, int32 Count);
	bool UpdateBuffIconStacksUI_Validate(const FGameplayTag& BuffTag, int32 Count) { return true; }

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
		void RemoveBuffIconFromUI(const FGameplayTag& BuffTag);
	void RemoveBuffIconFromUI_Implementation(const FGameplayTag& BuffTag);
	bool RemoveBuffIconFromUI_Validate(const FGameplayTag& BuffTag) { return true; }

	/*********************/
	/*   Input Handlers  */
	/*********************/

	UFUNCTION(BlueprintCallable)
		void OpenCharacterInfoMenu();

	UFUNCTION(BlueprintCallable)
		void OpenInventoryBagMenu();

	UFUNCTION(BlueprintCallable)
		void OpenSkillsMenu();

	UFUNCTION(BlueprintCallable)
		void OpenAbilityMenu();

	UFUNCTION(BlueprintCallable)
		void TryOpenEscapeMenu();

protected:

	// Which character class does player want (i.e Berserker, ranger, etc.)
	// TODO make this class on spawn not from GM
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class AP4PlayerCharacterBase> PlayerCharacterClass;

	// Floating Text Component Class to create for dmg text
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UFloatingTextWidgetComponent> FloatingTextClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		class UGameplayHudWidget* GameplayHUDWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		TSubclassOf<class UGameplayHudWidget> GameplayHUDWidgetClass;


	virtual void SetupInputComponent() override;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};


