// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Project4GameMode.h"
#include "P4OpenWorldGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4OpenWorldGameMode : public AProject4GameMode
{
	GENERATED_BODY()
	

public:

	virtual void GenericPlayerInitialization(AController* Controller) override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

};
