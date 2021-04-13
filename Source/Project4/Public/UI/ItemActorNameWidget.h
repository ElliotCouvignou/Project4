// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "Interactables/P4ItemBaseObject.h"
#include "ItemActorNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UItemActorNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetItemName(const FString& Name, EItemRank ItemRank);
	
};
