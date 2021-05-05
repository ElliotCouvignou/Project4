// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
//#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))

#define ISDEDICATED (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
#define ISLISTEN (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
#define ISSTANDALONE (GEngine->GetNetMode(GetWorld()) == NM_Standalone)
#define ISCLIENT (GEngine->GetNetMode(GetWorld()) == NM_Client)

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

#define ABILITY_BLOCK_AMOUNT 10
#define ABILITY_INPUT_OFFSET 3


// location of this struct might need to be moved if necessary
// ONLY CREATE A NEW ENUM IF: you want to bind an enum to a NEW inputAction event DisplayName
UENUM(BlueprintType)
enum class EP4AbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),  // "None" = no action binding reference, will be called in backend not users
	// 1 Confirm
	Confirm			UMETA(DisplayName = "ConfirmAbility"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "CancelAbility"),
	// Hotbar binding enums Currently used in BP as a reference to names, not used in GAS ability bindings like other enums
	UseAbility1		UMETA(DisplayName = "UseAbility1"),
	UseAbility2		UMETA(DisplayName = "UseAbility2"),
	UseAbility3		UMETA(DisplayName = "UseAbility3"),
	UseAbility4		UMETA(DisplayName = "UseAbility4"),
	UseAbility5		UMETA(DisplayName = "UseAbility5"),
	UseAbility6		UMETA(DisplayName = "UseAbility6"),
	UseAbility7		UMETA(DisplayName = "UseAbility7"),
	UseAbility8		UMETA(DisplayName = "UseAbility8"),
	UseAbility9		UMETA(DisplayName = "UseAbility9"),
	UseAbility10		UMETA(DisplayName = "UseAbility10"),

	SpecialAttack   UMETA(DisplayName = "SpecialAttack"), // RMB bind
	QAbility			UMETA(DisplayName = "QAbility"),
	EAbility			UMETA(DisplayName = "EAbility"),
	RAbility			UMETA(DisplayName = "RAbility"),

	// Essential Player abilities (e.g weapon swap, sprint, etc.) i.e used among all classes
	WeaponAttack    UMETA(DisplayName = "WeaponAttack"), // LMB "Auto Attack" bind
	InteractAbility	UMETA(DisplayName = "InteractAbility"),
	TabTarget       UMETA(DisplayName = "TabTarget"),
	ShowMouse		UMETA(DisplayName = "ShowMouse"),	
	ChangeWeapon		UMETA(DisplayName = "ChangeWeapon"),
	Sprint			UMETA(DisplayName = "Sprint")
};



// maybe move this an array somewhere else
UENUM(BlueprintType)
enum class EAbilityCategory: uint8
{
	Physical		UMETA(DisplayName = "Physical"),
	Magical			UMETA(DisplayName = "Magical"),
	Defensive		UMETA(DisplayName = "Defensive"),
	Utility			UMETA(DisplayName = "Utility")
};

static TArray<EAbilityCategory> GeneralSkillCategoryPool = { \
EAbilityCategory::Physical,
EAbilityCategory::Magical,
EAbilityCategory::Defensive,
EAbilityCategory::Utility };


// maybe move this too?
UENUM(BlueprintType)
enum class EClassAbilityPoolType : uint8
{
	// 0 None
	None					UMETA(DisplayName = "None"), // this is default for fresh character
	Hero					UMETA(DisplayName = "Hero"),    // has access to everything (mostly for testing maybe dont give to players)
	Berserker				UMETA(DisplayName = "Berserker"),  // "None"
	Ranger					UMETA(DisplayName = "Ranger")
};