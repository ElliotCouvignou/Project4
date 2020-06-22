// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#define ISDEDICATED (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
#define ISLISTEN (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
#define ISSTANDALONE (GEngine->GetNetMode(GetWorld()) == NM_Standalone)
#define ISCLIENT (GEngine->GetNetMode(GetWorld()) == NM_Client)


#define ABILITY_INPUT_OFFSET 3

// location of this struct might need to be moved if necessary
UENUM(BlueprintType)
enum class EP4AbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "ConfirmAbility"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "CancelAbility"),
	// 3-11 User input slots
	UseAbility1		UMETA(DisplayName = "UseAbility1"),
	UseAbility2		UMETA(DisplayName = "UseAbility2"),
	UseAbility3		UMETA(DisplayName = "UseAbility3"),
	UseAbility4		UMETA(DisplayName = "UseAbility4"),
	UseAbility5		UMETA(DisplayName = "UseAbility5"),
	UseAbility6		UMETA(DisplayName = "UseAbility6"),
	UseAbility7		UMETA(DisplayName = "UseAbility7"),
	UseAbility8		UMETA(DisplayName = "UseAbility8"),
	UseAbility9		UMETA(DisplayName = "UseAbility9")
};