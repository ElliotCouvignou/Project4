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
	UPROPERTY(BlueprintReadOnly, Replicated, EditAnywhere)
		FInventoryItemStruct InventoryItemStruct;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AP4ItemBaseActor();

	UFUNCTION(BlueprintCallable)
		virtual void GrantUniqueItemAbility();

	/* OnInteract == Pickup item */
		virtual void OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result) override;

	/* Used on actor spawn to set visuals and item info */
	UFUNCTION(BlueprintCallable)
		void SetItemStructAndStaticMesh(const FInventoryItemStruct& InputInventoryItemStruct);

	UFUNCTION(BlueprintCallable)
		FInventoryItemStruct& GetInventoryItemStructt() { return InventoryItemStruct; }

	UFUNCTION(BlueprintCallable)
		void UpdateInventoryItemStruct(const FInventoryItemStruct& InputIventoryItemStruct) { InventoryItemStruct = InputIventoryItemStruct; }


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
