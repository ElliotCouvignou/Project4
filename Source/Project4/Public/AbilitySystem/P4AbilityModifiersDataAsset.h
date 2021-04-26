// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/P4AbilityModifier.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "P4AbilityModifiersDataAsset.generated.h"



/**
 * 
 */
UCLASS()
class PROJECT4_API UP4AbilityModifiersDataAsset : public UDataAsset
{
	GENERATED_BODY()
	

public:
	UP4AbilityModifiersDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities | Pools")
		TMap<TSubclassOf<UP4GameplayAbility>, FP4AbilityModifierBaseInfoMapStruct> AbilityModifiers;
};
