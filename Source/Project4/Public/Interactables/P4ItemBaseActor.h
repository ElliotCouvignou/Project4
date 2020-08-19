// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/P4Interactable.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "GenericPlatform/GenericPlatform.h"

#include "P4ItemBaseActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4ItemBaseActor : public AP4Interactable
{
	GENERATED_BODY()

protected:


	/* created on spawn for handoffs */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FInventoryItemStruct InventoryItemStruct;



public:

	AP4ItemBaseActor();

	UFUNCTION(BlueprintCallable)
		virtual void GrantUniqueItemAbility();

	UFUNCTION(BlueprintCallable)
		virtual void OnItemEquipped();

	UFUNCTION(BlueprintCallable)
		virtual void OnItemUnequipped();

	/* OnInteract == Pickup item */
		virtual void OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result) override;


	UFUNCTION(BlueprintCallable)
		FInventoryItemStruct& GetInventoryItemStructt() { return InventoryItemStruct; }
};
