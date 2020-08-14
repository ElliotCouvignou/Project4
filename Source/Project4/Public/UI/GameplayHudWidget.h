// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project4Controller.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "GameplayHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UGameplayHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/* Callback funciton when viewport size changes, update's crosshair y offset length (in world space) */
	// No UFUNCTION since viewport isnt exposed to Unreal script
	void OnViewportResizedCallback(FViewport* ActiveViewport, uint32 UnknownUnsignedInt);

	// Offset in pixels (for 1920x1024 (default) scale)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crosshair")
		float CrosshairScreenYOffset;

	/***********************/
	/*     Progression     */
	/***********************/

	UFUNCTION(Category = Attributes)
		void UpdateCurrentXP(float NewXP);
	UFUNCTION(Category = Attributes)
		void UpdateMaxXP(float NewMaxXP);
	UFUNCTION(Category = Attributes)
		void UpdatePlayerLevel(float NewLevel);


	UFUNCTION(Category = Attributes)
		void UpdatePlayerName(const FString& NewName);


	/********************/
	/*     Resource     */
	/********************/

	UFUNCTION(Category = Attributes)
		void UpdateHealth(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateHealthMax(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateHealthRegen(float NewValue);

	UFUNCTION(Category = Attributes)
		void UpdateMana(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateManaMax(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateManaRegen(float NewValue);

	UFUNCTION(Category = Attributes)
		void UpdateEndurance(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateEnduranceMax(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateEnduranceRegen(float NewValue);

	/********************/
	/*     Defensive    */
	/********************/

	UFUNCTION(Category = Attributes)
		void UpdateArmor(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateMagicResistance(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateMovementSpeed(float NewValue);

	/********************/
	/*     Offensive    */
	/********************/

	UFUNCTION(Category = Attributes)
		void UpdateAttackPower(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateMagicPower(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateAttackSpeed(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateCritChance(float NewValue);
	UFUNCTION(Category = Attributes)
		void UpdateCritDamage(float NewValue);


	UFUNCTION(Category = "Ability Block")
		void SetAbilityHotbarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, Category = "Crosshair")
		void GetCrosshairGroundLocation(float MaxSearchDistance, FVector& EndLocation, FVector& StartLocation);


	/* Blueprint implementable functions, for widgets that dont 
		need a special class made for them in c++ so let BP's 
		handle from here (only need 1 function for this widget) */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetRespawnCountdown(float Duration);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NewAbilityErrorMessage(EAbilityErrorText ErrorType);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Crosshair")
		void SetNewTargetData(AActor* NewTarget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Buff Icons")
		void SendBuffIconToUI(const FGameplayTag& Bufftag, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Buff Icons")
		void UpdateBuffIconStacksUI(const FGameplayTag& BuffTag, int32 Count);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Buff Icons")
		void RemoveBuffIconFromUI(const FGameplayTag& BuffTag);


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Input Menus")
		void OpenCharacterInfoWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Input Menus")
		void OpenInventoryBagWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Input Menus")
		void OpenSkillsWidget();


protected:

	virtual void NativeConstruct() override;

	// gets created and filled out by BP
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attributes)
		class UXPBarWidget* XPBarWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attributes)
		class UResourceBarsWidget* ResourceBarsWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability Hotbar")
		class UAbilityHotbar* AbilityHotbar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff Icons")
		class UBuffIconsWidget* BuffIconsWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crosshair")
		class UCrosshairWidget* CrosshairWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crosshair")
		class UCharacterInfoWidget* CharacterInfoWidget;


	
};
