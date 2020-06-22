// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Project4.h"
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
		EP4AbilityInputID AbilityInputID = EP4AbilityInputID::None;

	// This is more for passive abilities that require an input slot even though users dont actually
	// do an input for it. Basically keeping up with GAS requirements
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
		EP4AbilityInputID AbilityID = EP4AbilityInputID::None;

	// when true fire off ability moment actor has it like some wort of Autocast
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		bool ActivateAbilityOnGranted = false;

	// Cooldown Tag UI widgets keep track of
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FGameplayTag CooldownTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UTexture2D* AbilityIcon;

	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override; 

	UFUNCTION(BlueprintCallable, Category = "Ability Functions")
	void SetAbilityInputID(EP4AbilityInputID NewInputID);

};
