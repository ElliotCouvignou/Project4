// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
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
class PROJECT4_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/* Constructor */
	UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer);

public:

	/*  is called before... well, an attribute's base value (so without any temporary modifiers) is changed.
	It would be unwise to use this for game logic, and is mostly there to allow you to describe stat clamping.*/
	void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/* same as above , but here you can define clamping with temporary modifiers instead.
	Either way, NewValue describes the new value of a changed stat, and FGameplayAttribute
	Attribute describes some info about the stat we're talking about */
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/* is a function that takes the data a GameplayEffectExecutionCalculation spits out
	(including which stats it wishes to modify, and by how much), and can then decide if the
	GameplayEffectExecutionCalculation is allowed to influence the AttributeSet in any way,
	by returning an appropriate bool. */
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// For Replicaiton 
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;


	// all attributes are essentially floats, can still have int behavior thought

	////////////////////////////////////
	/*         Progression Stats      */
	////////////////////////////////////

	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_Level, BlueprintReadWrite)
		FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Level) // Macro greates GetLevelAttribute() and GetLevel() which returns attribute and float value respectively
		UFUNCTION()
		void OnRep_Level(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Level, Previous); }

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


	/* Maximum Health */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_HealthMax, BlueprintReadWrite)
		FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, HealthMax)
	UFUNCTION()
		void OnRep_HealthMax(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HealthMax, Previous); }

	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_HealthRegen, BlueprintReadWrite)
		FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, HealthRegen)
	UFUNCTION()
		void OnRep_HealthRegen(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HealthRegen, Previous); }

	/* Basic Mana */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_Mana, BlueprintReadWrite)
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Mana)
	UFUNCTION()
		void OnRep_Mana(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Mana, Previous); }

	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_ManaMax, BlueprintReadWrite)
		FGameplayAttributeData ManaMax;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ManaMax)
	UFUNCTION()
		void OnRep_ManaMax(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ManaMax, Previous); }

	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_ManaRegen, BlueprintReadWrite)
		FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ManaRegen)
	UFUNCTION()
		void OnRep_ManaRegen(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ManaRegen, Previous); }

	
	////////////////////////////////////
	/*         Defensive Stats        */
	////////////////////////////////////

	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Defensive", EditAnywhere, ReplicatedUsing = OnRep_Armor, BlueprintReadWrite)
		FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Armor)
	UFUNCTION()
		void OnRep_Armor(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Armor, Previous); }

	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Defensive", EditAnywhere, ReplicatedUsing = OnRep_MagicResistance, BlueprintReadWrite)
		FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MagicResistance)
	UFUNCTION()
		void OnRep_MagicResistance(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MagicResistance, Previous); }



	////////////////////////////////////
	/*         Offensive Stats        */
	////////////////////////////////////
	

	/* Base Attack dmg */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_AttackPower, BlueprintReadWrite)
		FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, AttackPower)
	UFUNCTION()
		void OnRep_AttackPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, AttackPower, Previous); }

	/* Base magic dmg */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_MagicPower, BlueprintReadWrite)
		FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MagicPower)
	UFUNCTION()
		void OnRep_MagicPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MagicPower, Previous); }


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





