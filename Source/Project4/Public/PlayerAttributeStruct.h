// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAttributeStruct.generated.h"

// this struct holds all the info on the player's health, str, agi, etc
// Replication of struct happens on struct itself, no need for labeling it 
// for member varaiables. Only replicates changed variables
USTRUCT(BlueprintType)
struct FPlayerAttributes
{
	GENERATED_BODY();

	
	// HP essentials
	UPROPERTY(BlueprintReadWrite)
		float Health;
	UPROPERTY(BlueprintReadWrite)
		float HealthMax;
	UPROPERTY(BlueprintReadWrite)
		float HealthRegen;

	// mana essentials
	UPROPERTY(BlueprintReadWrite)
		float Mana;
	UPROPERTY(BlueprintReadWrite)
		float ManaMax;
	UPROPERTY(BlueprintReadWrite)
		float ManaRegen;

	// endurance essentials
	UPROPERTY(BlueprintReadWrite)
		float Endurance;
	UPROPERTY(BlueprintReadWrite)
		float EnduranceMax;
	UPROPERTY(BlueprintReadWrite)
		float EnduranceRegen;

	// Player Attributes 
	UPROPERTY(BlueprintReadWrite)
		int Strength;
	UPROPERTY(BlueprintReadWrite)
		int Stamina;
	UPROPERTY(BlueprintReadWrite)
		int Dexterity;
	UPROPERTY(BlueprintReadWrite)
		int Intelligence;
	UPROPERTY(BlueprintReadWrite)
		int Spirit;
	UPROPERTY(BlueprintReadWrite)
		int Wisdom;

	// Player combat stats
	UPROPERTY(BlueprintReadWrite)
		int Armor;
	UPROPERTY(BlueprintReadWrite)
		float BlockChance;
	UPROPERTY(BlueprintReadWrite)
		float CritChance;
	UPROPERTY(BlueprintReadWrite)
		float CritMultiplier;
	UPROPERTY(BlueprintReadWrite)
		float Haste;
	UPROPERTY(BlueprintReadWrite)
		float BaseAttackSpeed;
	UPROPERTY(BlueprintReadWrite)
		float CooldownReduciton;


	FPlayerAttributes() {
		
	}

};

