// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Project4.h"
#include "Project4Controller.h"
#include "GameplayTagContainer.h"
#include "P4GameplayAbility.generated.h"

/**
 *   This builds off the base Gameplay Ability class to allow for further functionality 
 */
UCLASS()
class PROJECT4_API UP4GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public: 
	UP4GameplayAbility();

	// Binds Ability to an input mapped from the enum which maps to project setting inputs.
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	//	EP4AbilityInputID AbilityInputID = EP4AbilityInputID::None;
	//
	//// This is more for passive abilities that require an input slot even though users dont actually
	//// do an input for it. Basically keeping up with GAS requirements
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	//	EP4AbilityInputID AbilityID = EP4AbilityInputID::None;

	// when true fire off ability moment actor has it like some wort of Autocast
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		bool ActivateAbilityOnGranted = false;

	// Cooldown Tag UI widgets keep track of
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FGameplayTag CooldownTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UTexture2D* AbilityIcon;

	/* tooltip text to show when on ability hotbar */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FText AbilityToolTipText;

	// Only if ability applies buff GE, not required to fill out
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FText BuffToolTipText;


	
	/*************************/
	/*       Utilities       */
	/*************************/

	
	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override; 

	UFUNCTION(BlueprintCallable, Category = "Utility | UI")
		void SendErrorMessageToUI(EAbilityErrorText ErrorType);

	/* Calls palyMontage ASC function that isn't blueprint exposed on default */
	UFUNCTION(BlueprintCallable, Category = "Utility | Animation")
		float PlayAnimationMontage(class AProject4Character* TargetActor, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* Montage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);


protected:

	/* virtual overrides */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
