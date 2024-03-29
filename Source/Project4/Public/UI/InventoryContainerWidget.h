// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/P4ItemBaseObject.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "InventoryContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UInventoryContainerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/* Simple handler to change empty bag slot to filled */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnInventorySlotUpdated(const UP4ItemBaseObject* NewitemInfo, int Index);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnItemRemoved(UP4ItemBaseObject* NewItemInfo, int InventoryIndex);

protected:

	virtual void NativeConstruct() override;
};
