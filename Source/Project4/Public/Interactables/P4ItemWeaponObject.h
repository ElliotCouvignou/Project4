// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemWeaponDataAsset.h"
#include "Interactables/P4ItemBaseObject.h"
#include "P4ItemWeaponObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECT4_API UP4ItemWeaponObject : public UP4ItemBaseObject
{
	GENERATED_BODY()
	

public:
	UP4ItemWeaponObject();

	// Not much addon just more specific on what armor type the armor item is
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		EWeaponType WeaponType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		EWeaponHandType WeaponHandType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		EItemRank ItemRank;

	/* Attack power for weapon, this gets sent to GE to fill out main or off hand wep attribute 
		(generated weapon doesn't know what hand we assign to leave here for set as caller magnitude on equip) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		float WeaponPower;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		float AttackInterval;

	/* Use a gameplay effect to fill out attribute buffs to reduce serverload at cost of game memory size (not that much hopefully) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		FGameplayEffectSpecHandle EquippedGameplayEffect;

	/* use for hardcoded GE's on specific items */
	UPROPERTY(BlueprintReadOnly, Category = "Item | Weapon | Generated")
		TSubclassOf<UGameplayEffect> PremadeGameplayEffectClass;

	/* Visual mesh (Not the appearence when dropped) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		USkeletalMesh* WeaponSkeletalMesh;

	/* Goes into independent weapon attack power for auto attacks */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		FTransform MainHandAttatchTransform;

	/* Goes into independent weapon attack power for auto attacks */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		FTransform OffHandAttatchTransform;


	virtual void InitializeFromDataAsset(UItemBaseDataAsset* Data) override;

	UFUNCTION(BlueprintCallable)
		void SetEquippedGameplayEffectByClass(TSubclassOf<UGameplayEffect> GEClass, UAbilitySystemComponent* ASC);
};
