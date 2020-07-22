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


	UFUNCTION()
		void UpdateHealth(float NewValue);
	UFUNCTION()
		void UpdateHealthMax(float NewValue);
	UFUNCTION()
		void UpdateHealthRegen(float NewValue);

	UFUNCTION()
		void UpdateMana(float NewValue);
	UFUNCTION()
		void UpdateManaMax(float NewValue);
	UFUNCTION()
		void UpdateManaRegen(float NewValue);

	UFUNCTION()
		void UpdateEndurance(float NewValue);
	UFUNCTION()
		void UpdateEnduranceMax(float NewValue);
	UFUNCTION()
		void UpdateEnduranceRegen(float NewValue);

	UFUNCTION()
		void UpdateCurrentXP(float NewXP);
	UFUNCTION()
		void UpdateMaxXP(float NewMaxXP);

	UFUNCTION()
		void UpdatePlayerLevel(float NewLevel);

	UFUNCTION()
		void SetAbilityHotbarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);


	/* Blueprint implementable functions, for widgets that dont 
		need a special class made for them in c++ so let BP's 
		handle from here (only need 1 function for this widget) */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetRespawnCountdown(float Duration);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NewAbilityErrorMessage(EAbilityErrorText ErrorType);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetNewTargetData(AActor* NewTarget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SendBuffIconToUI(const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateBuffIconStacksUI(const FGameplayTag& BuffTag, int32 Count);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void RemoveBuffIconFromUI(const FGameplayTag& BuffTag);


protected:
	// gets created and filled out by BP
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UXPBarWidget* XPBarWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UResourceBarsWidget* ResourceBarsWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UAbilityHotbar* AbilityHotbar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UBuffIconsWidget* BuffIconsWidget;
};
