// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/P4BaseAttributeSet.h"
#include "PlayerAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * The base of this code is heavily derived from https://github.com/tranek/GASDocumentation as
 * it's the most advanced and clear documentation on GAS I've seen so far
 */
UCLASS()
class PROJECT4_API UPlayerAttributeSet : public UP4BaseAttributeSet
{
	GENERATED_BODY()


public:

	/* Constructor */
	UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer);

	/*  is called before... well, an attribute's base value (so without any temporary modifiers) is changed.
	It would be unwise to use this for game logic, and is mostly there to allow you to describe stat clamping.*/
	//virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//
	///* same as above , but here you can define clamping with temporary modifiers instead.
	//Either way, NewValue describes the new value of a changed stat, and FGameplayAttribute
	//Attribute describes some info about the stat we're talking about */
	//virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//
	///* is a function that takes the data a GameplayEffectExecutionCalculation spits out
	//(including which stats it wishes to modify, and by how much), and can then decide if the
	//GameplayEffectExecutionCalculation is allowed to influence the AttributeSet in any way,
	//by returning an appropriate bool. */
	//virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	//
	// For Replicaiton 
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	//
	//// Adjusts current valued attributes when max valued attributes changes so that % stay constant
	//// This is the same idea Dota2 Uses for Health/Mana
	//virtual void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);


	// all attributes are essentially floats, can still have int behavior thought

	///////////////////////////////////////////////
	/*     Inherited Overrides and Accessors     */
	///////////////////////////////////////////////

		// all attributes are essentially floats, can still have int behavior thought

	////////////////////////////////////
	/*         Progression Stats      */
	////////////////////////////////////

	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_Experience, BlueprintReadWrite)
		FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Experience)
		UFUNCTION()
		void OnRep_Experience(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Experience, Previous); }

	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_ExperienceMax, BlueprintReadWrite)
		FGameplayAttributeData ExperienceMax;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ExperienceMax)
		UFUNCTION()
		void OnRep_ExperienceMax(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ExperienceMax, Previous); }


	/* to be referenced in Inventory component, current weight stored in component */
	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_CarryWeight, BlueprintReadWrite)
		FGameplayAttributeData CarryWeight;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, CarryWeight)
		UFUNCTION()
		void OnRep_CarryWeight(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, CarryWeight, Previous); }


	/* to be referenced in Inventory component, current weight stored in component */
	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_MaxCarryWeight, BlueprintReadWrite)
		FGameplayAttributeData MaxCarryWeight;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxCarryWeight)
		UFUNCTION()
		void OnRep_MaxCarryWeight(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxCarryWeight, Previous); }





};



