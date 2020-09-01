// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
//#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))

#define ISDEDICATED (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
#define ISLISTEN (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
#define ISSTANDALONE (GEngine->GetNetMode(GetWorld()) == NM_Standalone)
#define ISCLIENT (GEngine->GetNetMode(GetWorld()) == NM_Client)


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
	// Hotbar binding enums
	UseAbility1		UMETA(DisplayName = "UseAbility1"),
	UseAbility2		UMETA(DisplayName = "UseAbility2"),
	UseAbility3		UMETA(DisplayName = "UseAbility3"),
	UseAbility4		UMETA(DisplayName = "UseAbility4"),
	UseAbility5		UMETA(DisplayName = "UseAbility5"),
	UseAbility6		UMETA(DisplayName = "UseAbility6"),
	UseAbility7		UMETA(DisplayName = "UseAbility7"),
	UseAbility8		UMETA(DisplayName = "UseAbility8"),
	UseAbility9		UMETA(DisplayName = "UseAbility9"),

	// Essential Player abilities (e.g weapon swap, sprint, etc.) i.e used among all classes
	InteractAbility	UMETA(DisplayName = "InteractAbility"),
	WeaponAttack    UMETA(DisplayName = "WeaponAttack"), // LMB "Auto Attack" bind
	SpecialAttack   UMETA(DisplayName = "SpecialAttack"), // RMB bind
	TabTarget       UMETA(DisplayName = "TabTarget"),
	ShowMouse		UMETA(DisplayName = "ShowMouse"),	
	ChangeWeapon	UMETA(DisplayName = "ChangeWeapon"),
	Sprint			UMETA(DisplayName = "Sprint")
	

};