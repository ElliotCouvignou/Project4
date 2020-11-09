// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "P4AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4AIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Perception")
		class UAIPerceptionComponent* AIPerceptionComponent;


public:

	AP4AIControllerBase(const class FObjectInitializer& ObjectInitializer);
};
