// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/P4ItemBaseObject.h"
#include "P4ItemWeaponObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4ItemWeaponObject : public UP4ItemBaseObject
{
	GENERATED_BODY()
	

public:
	UP4ItemWeaponObject();

	// Not much addon just more specific on what armor type the armor item is
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Armor")
		EWeaponType WeaponType;

	/* Holds Active GE for pickup (Adds weight to carryweight player-attribute) */
	UPROPERTY(BlueprintReadOnly, Category = "Generated")
		FActiveGameplayEffectHandle ActiveGE;

	/* Use a gameplay effect to fill out attribute buffs to reduce serverload at cost of game memory size (not that much hopefully) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Armor")
		FGameplayEffectSpecHandle EquippedGameplayEffect;
};
