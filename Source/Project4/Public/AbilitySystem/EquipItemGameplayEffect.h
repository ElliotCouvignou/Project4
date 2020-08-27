// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "EquipItemGameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UEquipItemGameplayEffect : public UGameplayEffect
{
	
	

public:
	GENERATED_UCLASS_BODY()

	UEquipItemGameplayEffect(UDataTable* DataTable);
	
	//UEquipItemGameplayEffect(UDataTable* DataTable, EArmorType Armortype);
	
	UFUNCTION(BlueprintCallable)
		void SetupModifiersFromItemDataTable(UDataTable* DataTable);
	
	UFUNCTION(BlueprintCallable)
		void SetupGrantedTagsEffectArmorType(EArmorType Armortype);
};
