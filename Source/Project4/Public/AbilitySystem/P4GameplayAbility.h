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
struct PROJECT4_API FP4GEStackingStruct
{
	GENERATED_USTRUCT_BODY()
		// add more tuple params when needed		

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool Key;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stacking)
		EGameplayEffectStackingType	StackingType;

	/** Stack limit for StackingType */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stacking)
		int32 StackLimitCount;

	/** Policy for how the effect duration should be refreshed while stacking */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stacking)
		EGameplayEffectStackingDurationPolicy StackDurationRefreshPolicy;

	/** Policy for how the effect period should be reset (or not) while stacking */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stacking)
		EGameplayEffectStackingPeriodPolicy StackPeriodResetPolicy;

	/** Policy for how to handle duration expiring on this gameplay effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stacking)
		EGameplayEffectStackingExpirationPolicy StackExpirationPolicy;


	FP4GEStackingStruct()
	{
		Key = false;
	}

	FP4GEStackingStruct(const FP4GEStackingStruct& other)
	{
		Key = other.Key;
		StackingType = other.StackingType;
		StackLimitCount = other.StackLimitCount;
		StackDurationRefreshPolicy = other.StackDurationRefreshPolicy;
		StackPeriodResetPolicy = other.StackPeriodResetPolicy;
		StackExpirationPolicy = other.StackExpirationPolicy;
	}


	bool operator==(const FP4GEStackingStruct& r) const
	{
		return (Key == r.Key) && (StackingType == r.StackingType) && (StackLimitCount == r.StackLimitCount) && (StackDurationRefreshPolicy == r.StackDurationRefreshPolicy) && (StackPeriodResetPolicy == r.StackPeriodResetPolicy) && (StackExpirationPolicy == r.StackExpirationPolicy);
	}

};

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
		FP4GEStackingStruct Stacking;

	FP4GEExposedParametersStruct()
	{
		Stacking = FP4GEStackingStruct();
	}

	FP4GEExposedParametersStruct(const FP4GEExposedParametersStruct& other)
	{
		Stacking = other.Stacking;
	}

	bool operator==(const FP4GEExposedParametersStruct& r) const
	{
		return Stacking == r.Stacking;
	}

};

USTRUCT(BlueprintType)
struct PROJECT4_API FP4GECustomGameplayEffectStruct
{
	GENERATED_USTRUCT_BODY()
		// add more tuple params when needed		

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FP4GEExposedParametersStruct Params;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UGameplayEffect> BaseClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UGameplayEffect* GeneratedObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGameplayEffectSpec GeneratedSpec;
	

	FP4GECustomGameplayEffectStruct()
	{
		GeneratedSpec = FGameplayEffectSpec();
	}

	FP4GECustomGameplayEffectStruct(const FP4GEExposedParametersStruct& _Params, const TSubclassOf<UGameplayEffect> Class)
	{
		Params = _Params;
		BaseClass = Class;
	}

	FP4GECustomGameplayEffectStruct(const FP4GEExposedParametersStruct& _Params, const TSubclassOf<UGameplayEffect> Class, FGameplayEffectSpec& GE)
	{
		Params = _Params;
		BaseClass = Class;
		GeneratedSpec = GE;
	}

	FP4GECustomGameplayEffectStruct(const FP4GECustomGameplayEffectStruct& other)
	{
		Params = other.Params;
		BaseClass = other.BaseClass;
	}

	bool operator==(const FP4GECustomGameplayEffectStruct& r) const
	{
		return (BaseClass == r.BaseClass && Params == r.Params) || GeneratedObject == r.GeneratedObject;
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

	/* THIS IS ESSENTIAL, fill this out and have ability values depend on these base values so tooltip
	    is guaranteed to reflect actual values */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essential")
		UCurveTable* AbilityMagnitudes;

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

	// max range that the AI will be willnig to cast this ability 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float AI_CastRange = 0.f;

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

	/* Helper fucntion to be used in any ability that requires a target actor to cast on
	   This method first returns the AI or player selected actor (via tab). If no current actor selected by player
	   then do recursive cylinder traces at increasing radius. If this fails then return none/error */
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void TryGetTarget(float Range, bool AllowEnemies, bool AllowAllies, AProject4Character*& Result);

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAbilityMagnitude(FName RowName, const FString& ContextString, float& Result, float DefaultIfNotFound = 0.f) const;

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


	//UPROPERTY()
	//	TArray<FP4GECustomGameplayEffectStruct> GeneratedGameplayEffects;



	// TODO: move this to gameinstance or ASC if multiple abilities need to make same custom GE to stack
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers")
		TArray<TSubclassOf<class UP4AbilityModifierInfo>> AbilityModifiers;
};
