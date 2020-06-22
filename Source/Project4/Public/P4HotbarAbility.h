// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GenericPlatform/GenericPlatform.h"
#include "P4GameplayAbility.h"
#include "Project4Character.h"
#include "P4HotbarAbility.generated.h"

/**
 *  UNUSED IF ABILITYHOTBAR.H WORKS!!!
 */
UCLASS()
class PROJECT4_API UP4HotbarAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UP4HotbarAbility();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		TSubclassOf<UP4GameplayAbility> Ability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		AProject4Character* PlayerRef;

	// Not zero-indexed, BlockIndex = 1 means leftmost block
	UPROPERTY()
		int8 BlockIndex;
	
	// Grants player ability with Ability varaible and block index
	
};
