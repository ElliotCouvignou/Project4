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


private:

	/* class vars so we dont remake each post exec */
	FGameplayTag CritTag;
	FGameplayTag PhysicalDamageTag;
	FGameplayTag MagicDamageTag;

	FGameplayTagContainer DamageNumberContainerFilter;

public:

	/* Constructor */
	UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer);

	/*  is called before... well, an attribute's base value (so without any temporary modifiers) is changed.
	It would be unwise to use this for game logic, and is mostly there to allow you to describe stat clamping.*/
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/* same as above , but here you can define clamping with temporary modifiers instead.
	Either way, NewValue describes the new value of a changed stat, and FGameplayAttribute
	Attribute describes some info about the stat we're talking about */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/* is a function that takes the data a GameplayEffectExecutionCalculation spits out
	(including which stats it wishes to modify, and by how much), and can then decide if the
	GameplayEffectExecutionCalculation is allowed to influence the AttributeSet in any way,
	by returning an appropriate bool. */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// For Replicaiton 
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Adjusts current valued attributes when max valued attributes changes so that % stay constant
	// This is the same idea Dota2 Uses for Health/Mana
	virtual void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) override;
	
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

	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_ExperienceBounty, BlueprintReadWrite)
		FGameplayAttributeData ExperienceBounty; // Xp granted to to source on kill. Mobs need this filled out 
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ExperienceBounty) 
		UFUNCTION()
		void OnRep_ExperienceBounty(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ExperienceBounty, Previous); }


	////////////////////////////////////
	/*         Resource Stats        */
	////////////////////////////////////

	/* Basic Health/HitPoints */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_Health, BlueprintReadWrite)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Health) // Macro greates GetHealthAttribute() and GetHealth() which returns attribute and float value respectively
	UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& Previous)	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, Previous); }

	/* Basic Mana */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_Mana, BlueprintReadWrite)
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Mana)
	UFUNCTION()
		void OnRep_Mana(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Mana, Previous); }

	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_Endurance, BlueprintReadWrite)
		FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Endurance)
		UFUNCTION()
		void OnRep_Endurance(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Endurance, Previous); }
	

	////////////////////////////////////
	/*           Meta Stats           */
	////////////////////////////////////

	// This is more of a handoff/meta data variable for damage display back to damage source
	// Since damage from abilities is server-side, this variable need not to be replicated
	UPROPERTY(Category = "Player Attributes | Meta", EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Damage)

		// same as above but for healing
	UPROPERTY(Category = "Player Attributes | Meta", EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Heal;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Heal)

};





