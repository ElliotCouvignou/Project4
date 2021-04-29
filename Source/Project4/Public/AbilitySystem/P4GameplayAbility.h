// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Project4.h"
#include "Project4Controller.h"
#include "GameplayTagContainer.h"
#include "UI/AbilityTooltipWidget.h" // for tooltip struct info'
#include "AbilitySystem/GameplayEffects/P4AbilityModifierGameplayEffect.h"
#include "P4GameplayAbility.generated.h"



USTRUCT(BlueprintType)
struct PROJECT4_API FP4GENumberParamStruct
{
	GENERATED_USTRUCT_BODY()
		// add more tuple params when needed		

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Value;


	FP4GENumberParamStruct()
	{
		Key = false;
		Value = 0.f;
	}

	FP4GENumberParamStruct(bool _key, float _val)
	{
		Key = _key;
		Value = _val;
	}
};

/*
*  Struct of tuples to expose parameters to make more dynamic gameplay effects,
* Enable tuple bool to actually use this parameter, if not default to class
*/
USTRUCT(BlueprintType)
struct PROJECT4_API FP4GEExposedParametersStruct
{
	GENERATED_USTRUCT_BODY()
	// add more tuple params when needed		
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FP4GENumberParamStruct StackLimitCount;

	FP4GEExposedParametersStruct()
	{
		StackLimitCount = FP4GENumberParamStruct(false, 0);
	}

};


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

	/* Name of ability to be displayed to players */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Essential")
		FText AbilityName;

	// when true fire off ability moment actor has it like some wort of Autocast
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		bool ActivateAbilityOnGranted = false;

	// if true then this ability can have multiple charges that share cooldowns.
	// Functionality for charges to have independent CD is yet to be made
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		bool bHasCharges = false;

	// The max amount of charges if bHasCharges is true, else this is useless,
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		int MaxCharges = 0;

	// Cooldown Tag UI widgets keep track of
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FGameplayTag CooldownTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UTexture2D* AbilityIcon;

	// Only if ability applies buff GE, not required to fill out
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FText BuffToolTipText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector TargetDataLocation;

	/* Name of row to access ability descriptions for UI from description table */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Tooltip")
		FName AbilityDescriptionDataTableRowName;
	
	/*************************/
	/*       Utilities       */
	/*************************/

	
	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override; 

	UFUNCTION(BlueprintCallable, Category = "Utility | UI")
		void SendErrorMessageToUI(EAbilityErrorText ErrorType);

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void CreateCustomGameplayEffectSpec(TSubclassOf<UGameplayEffect> EffectClass, const FP4GEExposedParametersStruct& Params, const int& Level, FGameplayEffectSpecHandle& Result);

	//UFUNCTION(BlueprintCallable, Category = "Utility")
	//	void SetContitionalGameplayEffectSetByCallerByTag(TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag CallerTag, float Value);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void SendTargetDataToServer(UP4GameplayAbility* AbilityRef, const FVector& HitLocation);
	void SendTargetDataToServer_Implementation(UP4GameplayAbility* AbilityRef, const FVector& HitLocation);
	bool SendTargetDataToServer_Validate(UP4GameplayAbility* AbilityRef, const FVector& HitLocation) { return true; }

	UFUNCTION(BlueprintCallable)
		FRotator GetLookatRotation(float Range, FVector SourceLocation);

};
