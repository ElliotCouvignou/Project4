// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/P4Interactable.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Components/WidgetComponent.h"

#include "P4ItemBaseActor.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT4_API AP4ItemBaseActor : public AP4Interactable
{
	GENERATED_BODY()



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visuals, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetComponent;


	/* created on spawn for handoffs */
	UPROPERTY(BlueprintReadOnly, Replicated, EditAnywhere, Category = "ItemObject")
		UP4ItemBaseObject* ItemObject;

	/* Should be left null unless you want to drag and drop item to be created in the level */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ItemObject | Optional")
		UItemBaseDataAsset* ItemDataAsset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essentials")
		TSubclassOf<class UItemActorNameWidget> ItemNameWidgetClass;

	UFUNCTION()
		void SetWidgetName();

	// in case the item gets spawn midair, move it to the ground
	UFUNCTION()
		void MoveToGround();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AP4ItemBaseActor();

	UFUNCTION(BlueprintCallable)
		virtual void GrantUniqueItemAbility();

	/* OnInteract == Pickup item */
		virtual void OnInteract(const AP4PlayerCharacterBase* SourceActor) override;


	void SetInventoryItemObject(UP4ItemBaseObject** nItemObject) { ItemObject = *nItemObject; }

	UFUNCTION(BlueprintCallable)
		UP4ItemBaseObject* GetInventoryItemStrucObject() { return ItemObject; }


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
