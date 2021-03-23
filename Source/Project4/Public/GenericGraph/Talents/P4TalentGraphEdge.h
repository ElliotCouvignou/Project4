// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GenericGraphEdge.h"
#include "P4TalentGraphEdge.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT4_API UP4TalentGraphEdge : public UGenericGraphEdge
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Pool")
		FText Selection;
};
