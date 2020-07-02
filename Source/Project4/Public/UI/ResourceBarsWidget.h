// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceBarsWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UResourceBarsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdatePlayerLevel(float NewLevel);
};
