// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/* Constructor */
	UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer);

public:
	// all attributes are essentially floats, can still have int behavior thought

	/* Basic Health/HitPoints */
	UPROPERTY(Category = "Player Attributes | Health", EditAnywhere, ReplicatedUsing = OnRep_Health, BlueprintReadWrite)
		FGameplayAttributeData Health;
	UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& Previous)	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, Previous);
	}

	/* Maximum Health */
	UPROPERTY(Category = "Player Attributes | Health", EditAnywhere, ReplicatedUsing = OnRep_HealthMax, BlueprintReadWrite)
		FGameplayAttributeData HealthMax;
	UFUNCTION()
		void OnRep_HealthMax(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HealthMax, Previous); }

	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Health", EditAnywhere, ReplicatedUsing = OnRep_HealthRegen, BlueprintReadWrite)
		FGameplayAttributeData HealthRegen;
	UFUNCTION()
		void OnRep_HealthRegen(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HealthRegen, Previous); }



	////////////////////////////////////
	/*         Defensive Stats        */
	////////////////////////////////////



	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Defensive", EditAnywhere, ReplicatedUsing = OnRep_Armor, BlueprintReadWrite)
		FGameplayAttributeData Armor;
	UFUNCTION()
		void OnRep_Armor(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Armor, Previous); }

	/* Health Regeneration per second */
	UPROPERTY(Category = "Player Attributes | Defensive", EditAnywhere, ReplicatedUsing = OnRep_MagicResistance, BlueprintReadWrite)
		FGameplayAttributeData MagicResistance;
	UFUNCTION()
		void OnRep_MagicResistance(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MagicResistance, Previous); }



	////////////////////////////////////
	/*         Offensive Stats        */
	////////////////////////////////////
	

	/* Base Attack dmg */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_AttackPower, BlueprintReadWrite)
		FGameplayAttributeData AttackPower;
	UFUNCTION()
		void OnRep_AttackPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, AttackPower, Previous); }

	/* Base magic dmg */
	UPROPERTY(Category = "Player Attributes | Offensive", EditAnywhere, ReplicatedUsing = OnRep_MagicPower, BlueprintReadWrite)
		FGameplayAttributeData MagicPower;
	UFUNCTION()
		void OnRep_MagicPower(const FGameplayAttributeData& Previous) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MagicPower, Previous); }






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

	// -------------------------------------------------
	
	// Attribute Grabber/Verifier
	static FGameplayAttribute HealthAttribute();
	static FGameplayAttribute HealthMaxAttribute();
	static FGameplayAttribute HealthRegenAttribute();

	static FGameplayAttribute ArmrorAttribute();
	static FGameplayAttribute MagicResistanceAttribute();

	static FGameplayAttribute AttackPowerAttribute();
	static FGameplayAttribute MagicPowerAttribute();

};





