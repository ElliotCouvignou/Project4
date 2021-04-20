// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuWidget.generated.h"




// THIS HOLDs INFO ABOUT RARITY 
UENUM(BlueprintType)
enum class ESettingsMenuCategory : uint8
{
	// 0 None
	Gameplay			UMETA(DisplayName = "Gameplay"),
	Controls			UMETA(DisplayName = "Controls"),
	Audio				UMETA(DisplayName = "Audio"),	
	Video				UMETA(DisplayName = "Video")
};



/**
 * 
 */
UCLASS()
class PROJECT4_API USettingsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	


};
