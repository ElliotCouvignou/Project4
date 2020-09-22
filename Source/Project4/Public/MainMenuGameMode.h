// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMainMenuGameMode();

protected:
	virtual void BeginPlay() override;
};
