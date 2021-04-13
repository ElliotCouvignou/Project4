// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemArmorDataAsset.h"
#include "Interactables/P4ItemBaseObject.h"
#include "P4ItemArmorObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECT4_API UP4ItemArmorObject : public UP4ItemBaseObject
{
	GENERATED_BODY()
	

public:
	UP4ItemArmorObject();
	
	// Not much addon just more specific on what armor type the armor item is
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Armor")
		EArmorType ArmorType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		EItemRank ItemRank;

	/* Use a gameplay effect to fill out attribute buffs to reduce serverload at cost of game memory size (not that much hopefully) */
	UPROPERTY(BlueprintReadWrite, Category = "Item | Armor | Generated")
		FGameplayEffectSpecHandle EquippedGameplayEffect;

	/* use for hardcoded GE's on specific items */
	UPROPERTY(BlueprintReadOnly, Category = "Item | Armor")
		TSubclassOf<UGameplayEffect> PremadeGameplayEffectClass;


	virtual void InitializeFromDataAsset(UItemBaseDataAsset* Data) override;

	UFUNCTION(BlueprintCallable)
		void SetEquippedGameplayEffectByClass(TSubclassOf<UGameplayEffect> GEClass, UAbilitySystemComponent* ASC);
};
