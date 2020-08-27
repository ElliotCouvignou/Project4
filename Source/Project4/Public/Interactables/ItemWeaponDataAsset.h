// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "ItemWeaponDataAsset.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),     // "None" for empty slots (this shouldnt be necessary at this depth)
	
	// one hander stuff
	Sword1H			UMETA(DisplayName = "Sword1H"),  // "One handed sword"
	Axe1H			UMETA(DisplayName = "Axe1H"),
	Dagger			UMETA(DisplayName = "Dagger"),   // 2h daggers dont exist
	Wand			UMETA(DisplayName = "Wand"),	 // 2h wands are staffs
	Pistol			UMETA(DisplayName = "Pistol"),
	Crossbow1H		UMETA(DisplayName = "Crossbow1H"),

	// offhand only
	Orb				UMETA(DisplayName = "Orb"),		 // TODO: find better name for magic off-hand thingy
	Shield			UMETA(DisplayName = "Shield"),

	// Two Hander Stuff
	Sword2H			UMETA(DisplayName = "Sword2H"),  
	Axe2H			UMETA(DisplayName = "Axe2H"),
	Staff			UMETA(DisplayName = "Staff"),
	Rifle			UMETA(DisplayName = "Rifle"),
	Bow				UMETA(DisplayName = "Bow")
};

/**
 * 
 */
UCLASS()
class PROJECT4_API UItemWeaponDataAsset : public UItemBaseDataAsset
{
	GENERATED_BODY()


public:

	UItemWeaponDataAsset();

	// Not much addon just more specific on what armor type the armor item is
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		EWeaponType WeaponType;

	/* Visual mesh (Not the appearence when dropped) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		USkeletalMesh* WeaponSkeletalMesh;

	/* Although this goes into attributes it cannot go into the GE since it will need to know which attribute (main or off) to addon to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		float AttackInterval;

	/* Use a gameplay effect to fill out attribute buffs to reduce serverload at cost of game memory size (not that much hopefully) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		TSubclassOf<UGameplayEffect> EquippedGameplayEffect;



	/*********************/
	/* Utility Funcitons */
	/*********************/
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAttributeNamesAndValuesFromGameplayEffect(TArray<FAttributeDataUIStruct>& OutDataArray);

	
};
