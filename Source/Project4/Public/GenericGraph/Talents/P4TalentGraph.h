// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GenericGraph.h"
#include "P4TalentGraph.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT4_API UP4TalentGraph : public UGenericGraph
{
	GENERATED_BODY()

public:
	UP4TalentGraph();

	UPROPERTY(EditDefaultsOnly, Category = "Ability Pool")
		FLinearColor TalentGraphColor;
};
