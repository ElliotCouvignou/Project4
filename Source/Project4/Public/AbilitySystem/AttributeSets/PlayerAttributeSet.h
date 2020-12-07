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

	// all attributes are essentially floats, can still have int behavior thought

	///////////////////////////////////////////////
	/*     Inherited Overrides and Accessors     */
	///////////////////////////////////////////////

	////////////////////////////////////
	/*         Progression Stats      */
	////////////////////////////////////

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Level) // Macro greates GetLevelAttribute() and GetLevel() which returns attribute and float value respectively
		virtual void OnRep_Level(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Level, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ExperienceBounty)
		virtual void OnRep_ExperienceBounty(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ExperienceBounty, Previous); }

	////////////////////////////////////
	/*         Resource Stats        */
	////////////////////////////////////

	/* Basic Health/HitPoints */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Health) // Macro greates GetHealthAttribute() and GetHealth() which returns attribute and float value respectively	
		virtual void OnRep_Health(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, Previous); }

	/* Basic Mana */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Mana)
		virtual void OnRep_Mana(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Mana, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Endurance)
		virtual void OnRep_Endurance(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Endurance, Previous); }

	/* Bonus health for items, Current Health for Players */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, HealthMax)
		virtual void OnRep_HealthMax(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HealthMax, Previous); }

	/* Health Regeneration per second */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, HealthRegen)
		virtual void OnRep_HealthRegen(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HealthRegen, Previous); }

	/* Basic Mana */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ManaMax)
		virtual void OnRep_ManaMax(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ManaMax, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ManaRegen)
		virtual void OnRep_ManaRegen(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ManaRegen, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, EnduranceMax)
		virtual void OnRep_EnduranceMax(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, EnduranceMax, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, EnduranceRegen)
		virtual void OnRep_EnduranceRegen(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, EnduranceRegen, Previous); }

	////////////////////////////////////
	/*			Base Stats			  */
	////////////////////////////////////

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Strength)
		virtual void OnRep_Strength(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Strength, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Dexterity)
		virtual void OnRep_Dexterity(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Dexterity, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Intelligence)
		virtual void OnRep_Intelligence(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Intelligence, Previous); }

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Spirit)
		virtual void OnRep_Spirit(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Spirit, Previous); }


	////////////////////////////////////
	/*         Defensive Stats        */
	////////////////////////////////////

	/* Health Regeneration per second */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Armor)
		virtual void OnRep_Armor(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Armor, Previous); }

	/* Health Regeneration per second */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MagicResistance)
		virtual void OnRep_MagicResistance(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MagicResistance, Previous); }

	/* For items only special ones will grant MS bonuses if any*/
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MovementSpeed)
		virtual void OnRep_MovementSpeed(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MovementSpeed, Previous); }

	////////////////////////////////////
	/*         Offensive Stats        */
	////////////////////////////////////

	/* Base/Bonus Attack dmg */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, AttackPower)
		virtual void OnRep_AttackPower(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, AttackPower, Previous); }

	/* Base magic dmg */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MagicPower)
		virtual void OnRep_MagicPower(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MagicPower, Previous); }


	/* Independent Weapon of main hand (Main hand) auto Weapon (so main hand AA damage isnt equal to offhand)  */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MainHandWeaponPower)
		virtual void OnRep_MainHandWeaponPower(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MainHandWeaponPower, Previous); }

	/* Interval of main hand (Main hand) auto attack */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MainHandAttackInterval)
		virtual void OnRep_MainHandAttackInterval(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MainHandAttackInterval, Previous); }

	/* */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, OffHandWeaponPower)
		virtual void OnRep_OffHandWeaponPower(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, OffHandWeaponPower, Previous); }

	/* */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, OffHandAttackInterval)
		virtual void OnRep_OffHandAttackInterval(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, OffHandAttackInterval, Previous); }


	/* % decimal measurement for increase attack speed (e.g 0.3 = 30%), applied to weapon attack interval for total auto attack rate */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, AttackSpeed)
		virtual void OnRep_AttackSpeed(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, AttackSpeed, Previous); }

	/* % (0-1) measurement for Crit % chance MUST BE BETWEEN 0 and 1 */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, CritChance)
		virtual void OnRep_CritChance(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, CritChance, Previous); }

	/* % Damage increase when crit applies  */
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, CritDamage)
		virtual void OnRep_CritDamage(const FGameplayAttributeData& Previous) override { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, CritDamage, Previous); }


	/****************************** NEW STUFF ***********************/

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





