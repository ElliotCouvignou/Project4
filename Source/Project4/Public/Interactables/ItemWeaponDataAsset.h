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
	
	// one/2h hander stuff
	Sword			UMETA(DisplayName = "Sword"),  // "One handed sword"
	Axe				UMETA(DisplayName = "Axe"),
	Gun				UMETA(DisplayName = "Gun"),
	Crossbow		UMETA(DisplayName = "Crossbow"),

	// 1h exclusively
	Dagger			UMETA(DisplayName = "Dagger"),   // 2h daggers dont exist
	Wand			UMETA(DisplayName = "Wand"),	 // 2h wands are staffs

	// offhand only
	Orb				UMETA(DisplayName = "Orb"),		 // TODO: find better name for magic off-hand thingy
	Shield			UMETA(DisplayName = "Shield"),
	Quiver			UMETA(DisplayName = "Quiver"),

	//2h only
	Staff			UMETA(DisplayName = "Staff"),
	Bow				UMETA(DisplayName = "Bow")
};

/* Hand type of weapon, (offhand only, takes both hands, one hand) */
UENUM(BlueprintType)
enum class EWeaponHandType : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),     // "None" for empty slots (this shouldnt be necessary at this depth)
	OffhandOnly		UMETA(DisplayName = "Offhand Only (1H)"),    // 
	AnyOneHand		UMETA(DisplayName = "AnyOneHand (1H)"),
	BothHands		UMETA(DisplayName = "BothHands (2H)")		// wielding this will take both hands
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		EWeaponHandType WeaponHandType;

	/* Visual mesh (Not the appearence when dropped) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		USkeletalMesh* WeaponSkeletalMesh;

	/* Although this goes into attributes it cannot go into the GE since it will need to know which attribute (main or off) to addon to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		float AttackInterval;

	/* Goes into independent weapon attack power for auto attacks */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		float WeaponPower;

	/* Use a gameplay effect to fill out attribute buffs to reduce serverload at cost of game memory size (not that much hopefully) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Weapon")
		TSubclassOf<UGameplayEffect> EquippedGameplayEffect;



	/*********************/
	/* Utility Funcitons */
	/*********************/
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAttributeNamesAndValuesFromGameplayEffect(TArray<FAttributeDataUIStruct>& OutDataArray);

	
};
