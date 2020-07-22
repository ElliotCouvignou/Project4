// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "Project4Controller.generated.h"


// location of this struct might need to be moved if necessary
// ONLY CREATE A NEW ENUM IF: you want to bind an enum to a NEW inputAction event DisplayName
UENUM(BlueprintType)
enum class EAbilityErrorText : uint8
{
	OutOfRange			UMETA(DisplayName = "OutOfRange"),
	OutOfMana			UMETA(DisplayName = "OutOfMana"),
	NoTarget			UMETA(DisplayName = "NoTarget"),
	InvalidTarget       UMETA(DisplayName = "InvalidTarget"),
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

	// creates a widget based on gameplayHUDwidgetclass
		void CreateMainHUDWidget();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		class UGameplayHudWidget* GetMainHUDWidget();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		TSubclassOf<AP4PlayerCharacterBase> GetPlayerCharacterClass();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void SetPlayerCharacterClass(TSubclassOf<AP4PlayerCharacterBase> CharacterClass);

	/************************************/
	/* Server RPC/Functions From Client */
	/************************************/

	// Called after respawn button pressed
	// need server for gamemode access to respawn
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ClientRequestRespawn();
	void ClientRequestRespawn_Implementation();
	bool ClientRequestRespawn_Validate() { return true; }

	/*******************************/
	/* Client RPC/Functions for UI */
	/*******************************/

	// Server calls this fucntion to display damage numbers to a player
	UFUNCTION(Client, Reliable, WithValidation)
		void DisplayDamageNumber(float DamageValue, AProject4Character* TargetCharacter);
	void DisplayDamageNumber_Implementation(float DamageValue, AProject4Character* TargetCharacter);
	bool DisplayDamageNumber_Validate(float DamageValue, AProject4Character* TargetCharacter) { return true; }

	// Server calls this fucntion to display damage numbers to a player
	UFUNCTION(Client, Reliable, WithValidation)
		void DisplayHealNumber(float HealValue, AProject4Character* TargetCharacter);
	void DisplayHealNumber_Implementation(float HealValue, AProject4Character* TargetCharacter);
	bool DisplayHealNumber_Validate(float HealValue, AProject4Character* TargetCharacter) { return true; }

	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateUICurrentXP(float NewXP);
	void UpdateUICurrentXP_Implementation(float NewXP);
	bool UpdateUICurrentXP_Validate(float NewXP) { return true; }

	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateUIMaxXP(float NewMaxXP);
	void UpdateUIMaxXP_Implementation(float NewMaxXP);
	bool UpdateUIMaxXP_Validate(float NewMaxXP) { return true; }

	// Called on Ding! Gratz
	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateUILevel(float NewLevel);
	void UpdateUILevel_Implementation(float NewLevel);
	bool UpdateUILevel_Validate(float NewLevel) { return true; }

	UFUNCTION(Client, Reliable, WithValidation)
		void SetUIRespawnCountdown(float Duration);
	void SetUIRespawnCountdown_Implementation(float Duration);
	bool SetUIRespawnCountdown_Validate(float Duration) { return true; }

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

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};


