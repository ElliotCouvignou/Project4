// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMainMenuHUD();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		TSubclassOf<UUserWidget> MainMenuWidgetClass;


};
