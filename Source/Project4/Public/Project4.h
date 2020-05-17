// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#define ISDEDICATED (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
#define ISLISTEN (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
#define ISSTANDALONE (GEngine->GetNetMode(GetWorld()) == NM_Standalone)
#define ISCLIENT (GEngine->GetNetMode(GetWorld()) == NM_Client)