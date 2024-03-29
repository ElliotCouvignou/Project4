// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "P4BaseAttributeSet.generated.h"


// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Purely Stat Attributes that can be inherited by both players and items.
 * This set should ONLY BE USED as a parent, not on their own as items/players deal with
 * these attr changes differently
 */
UCLASS()
class PROJECT4_API UP4BaseAttributeSet : public UAttributeSet
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
	UP4BaseAttributeSet(const FObjectInitializer& ObjectInitializer);

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
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	// Adjusts current valued attributes when max valued attributes changes so that % stay constant
	// This is the same idea Dota2 Uses for Health/Mana
	virtual void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);


	/****************/

	// all attributes are essentially floats, can still have int behavior thought

	////////////////////////////////////
	/*         Progression Stats      */
	////////////////////////////////////
	
	/* For Items: Level = level requirement, For Players: Level = your level */
	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_Level, BlueprintReadWrite)
		FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Level) // Macro greates GetLevelAttribute() and GetLevel() which returns attribute and float value respectively
		UFUNCTION()
		virtual void OnRep_Level(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Level, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Progression", EditAnywhere, ReplicatedUsing = OnRep_ExperienceBounty, BlueprintReadWrite)
		FGameplayAttributeData ExperienceBounty; // Xp granted to to source on kill. Mobs need this filled out 
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, ExperienceBounty)
		UFUNCTION()
		virtual void OnRep_ExperienceBounty(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, ExperienceBounty, Previous); }
	
	////////////////////////////////////
	/*         Resource Stats        */
	////////////////////////////////////
	
	/* Basic Health/HitPoints */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_Health, BlueprintReadWrite)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Health) // Macro greates GetHealthAttribute() and GetHealth() which returns attribute and float value respectively
		UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Health, Previous); }
	
	/* Basic Mana */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_Mana, BlueprintReadWrite)
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Mana)
		UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Mana, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_Endurance, BlueprintReadWrite)
		FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Endurance)
		UFUNCTION()
		virtual void OnRep_Endurance(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Endurance, Previous); }
	
	/* Bonus health for items, Current Health for Players */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_HealthMax, BlueprintReadWrite)
		FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, HealthMax)
		UFUNCTION()
		virtual void OnRep_HealthMax(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, HealthMax, Previous); }
	
	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_HealthRegen, BlueprintReadWrite)
		FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, HealthRegen)
		UFUNCTION()
		virtual void OnRep_HealthRegen(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, HealthRegen, Previous); }
	
	/* Basic Mana */
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_ManaMax, BlueprintReadWrite)
		FGameplayAttributeData ManaMax;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, ManaMax)
		UFUNCTION()
		virtual void OnRep_ManaMax(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, ManaMax, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_ManaRegen, BlueprintReadWrite)
		FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, ManaRegen)
		UFUNCTION()
		virtual void OnRep_ManaRegen(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, ManaRegen, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_EnduranceMax, BlueprintReadWrite)
		FGameplayAttributeData EnduranceMax;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, EnduranceMax)
		UFUNCTION()
		virtual void OnRep_EnduranceMax(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, EnduranceMax, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_EnduranceRegen, BlueprintReadWrite)
		FGameplayAttributeData EnduranceRegen;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, EnduranceRegen)
		UFUNCTION()
		virtual void OnRep_EnduranceRegen(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, EnduranceRegen, Previous); }
	
	////////////////////////////////////
	/*			Base Stats			  */
	////////////////////////////////////
	
	UPROPERTY(Category = "Player Attributes | Base", EditAnywhere, ReplicatedUsing = OnRep_Strength, BlueprintReadWrite)
		FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Strength)
		UFUNCTION()
		virtual void OnRep_Strength(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Strength, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Base", EditAnywhere, ReplicatedUsing = OnRep_Dexterity, BlueprintReadWrite)
		FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Dexterity)
		UFUNCTION()
		virtual void OnRep_Dexterity(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Dexterity, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Base", EditAnywhere, ReplicatedUsing = OnRep_Intelligence, BlueprintReadWrite)
		FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Intelligence)
		UFUNCTION()
		virtual void OnRep_Intelligence(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Intelligence, Previous); }
	
	UPROPERTY(Category = "Player Attributes | Base", EditAnywhere, ReplicatedUsing = OnRep_Spirit, BlueprintReadWrite)
		FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Spirit)
		UFUNCTION()
		virtual void OnRep_Spirit(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Spirit, Previous); }
	
	
	////////////////////////////////////
	/*         Defensive Stats        */
	////////////////////////////////////
	
	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Defensive", EditAnywhere, ReplicatedUsing = OnRep_Armor, BlueprintReadWrite)
		FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Armor)
		UFUNCTION()
		virtual void OnRep_Armor(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, Armor, Previous); }
	
	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Defensive", EditAnywhere, ReplicatedUsing = OnRep_MagicResistance, BlueprintReadWrite)
		FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, MagicResistance)
		UFUNCTION()
		virtual void OnRep_MagicResistance(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, MagicResistance, Previous); }
	
	/* For items only special ones will grant MS bonuses if any*/
	UPROPERTY(Category = "Player Attributes | Resource", EditAnywhere, ReplicatedUsing = OnRep_MovementSpeed, BlueprintReadWrite)
		FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, MovementSpeed)
		UFUNCTION()
		virtual void OnRep_MovementSpeed(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, MovementSpeed, Previous); }
	
	////////////////////////////////////
	/*         Offensive Stats        */
	////////////////////////////////////
	
	/* Base/Bonus Attack dmg */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_AttackPower, BlueprintReadWrite)
		FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, AttackPower)
		UFUNCTION()
		virtual void OnRep_AttackPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, AttackPower, Previous); }
	
	/* Base magic dmg */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_MagicPower, BlueprintReadWrite)
		FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, MagicPower)
		UFUNCTION()
		virtual void OnRep_MagicPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, MagicPower, Previous); }

	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_HealPower, BlueprintReadWrite)
		FGameplayAttributeData HealPower;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, HealPower)
		UFUNCTION()
		virtual void OnRep_HealPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, HealPower, Previous); }
	
	
	/* Independent Weapon of main hand (Main hand) auto Weapon (so main hand AA damage isnt equal to offhand)  */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_MainHandWeaponPower, BlueprintReadWrite)
		FGameplayAttributeData MainHandWeaponPower;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, MainHandWeaponPower)
		UFUNCTION()
		virtual void OnRep_MainHandWeaponPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, MainHandWeaponPower, Previous); }
	
	/* Interval of main hand (Main hand) auto attack */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_MainHandAttackInterval, BlueprintReadWrite)
		FGameplayAttributeData MainHandAttackInterval;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, MainHandAttackInterval)
		UFUNCTION()
		virtual void OnRep_MainHandAttackInterval(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, MainHandAttackInterval, Previous); }
	
	/* */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_OffHandWeaponPower, BlueprintReadWrite)
		FGameplayAttributeData OffHandWeaponPower;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, OffHandWeaponPower)
		UFUNCTION()
		virtual void OnRep_OffHandWeaponPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, OffHandWeaponPower, Previous); }
	
	/* */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_OffHandAttackInterval, BlueprintReadWrite)
		FGameplayAttributeData OffHandAttackInterval;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, OffHandAttackInterval)
		UFUNCTION()
		virtual void OnRep_OffHandAttackInterval(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, OffHandAttackInterval, Previous); }
	
	
	/* % decimal measurement for increase attack speed (e.g 0.3 = 30%), applied to weapon attack interval for total auto attack rate */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_AttackSpeed, BlueprintReadWrite)
		FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, AttackSpeed)
		UFUNCTION()
		virtual void OnRep_AttackSpeed(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, AttackSpeed, Previous); }
	
	/* % (0-1) measurement for Crit % chance MUST BE BETWEEN 0 and 1 */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_CritChance, BlueprintReadWrite)
		FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, CritChance)
		UFUNCTION()
		virtual void OnRep_CritChance(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, CritChance, Previous); }
	
	/* % Damage increase when crit applies  */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_CritDamage, BlueprintReadWrite)
		FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, CritDamage)
		UFUNCTION()
		virtual void OnRep_CritDamage(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UP4BaseAttributeSet, CritDamage, Previous); }
	
	////////////////////////////////////
	/*           Meta Stats           */
	////////////////////////////////////
	
	// This is more of a handoff/meta data variable for damage display back to damage source
	// Since damage from abilities is server-side, this variable need not to be replicated
	UPROPERTY(Category = "Player Attributes | Meta", EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Damage)
	
		// same as above but for healing
		UPROPERTY(Category = "Player Attributes | Meta", EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Heal;
	ATTRIBUTE_ACCESSORS(UP4BaseAttributeSet, Heal)
};
