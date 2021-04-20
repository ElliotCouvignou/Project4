// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "P4SettingsSave.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4SettingsSave : public USaveGame
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintReadWrite, Category = "Controls")
		float MouseSensitivity;
	


	UPROPERTY(BlueprintReadWrite, Category = "Audio")
		float Master;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
		float Effects;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
		float Music;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
		float Ambient;
};
