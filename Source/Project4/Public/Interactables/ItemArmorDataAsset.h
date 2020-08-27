// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBaseDataAsset.h"
#include "ItemArmorDataAsset.generated.h"


// this determins what armor is a helmet/glove/boot/etc.
UENUM(BlueprintType)
enum class EArmorType : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),  // "None" for empty slots (this shouldnt be necessary at this depth)
	Helmet			UMETA(DispalyName = "Helmet"),
	Necklace		UMETA(DispalyName = "Necklace"),
	Shoulder		UMETA(DispalyName = "Shoulder"),
	Chest			UMETA(DispalyName = "Chest"),
	Back			UMETA(DispalyName = "Back"),
	Gloves			UMETA(DispalyName = "Gloves"),
	Belt			UMETA(DispalyName = "Belt"),
	Legs			UMETA(DispalyName = "Legs"),
	Boots			UMETA(DispalyName = "Boots"),
	Ring			UMETA(DispalyName = "Ring"),
	Bag				UMETA(DispalyName = "Bag")
};



/**
 * 
 */
UCLASS(BlueprintType)
class PROJECT4_API UItemArmorDataAsset : public UItemBaseDataAsset
{
	GENERATED_BODY()
	
public:

	UItemArmorDataAsset();

	// Not much addon just more specific on what armor type the armor item is
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Armor")
		EArmorType ArmorType;

	/* Use a gameplay effect to fill out attribute buffs to reduce serverload at cost of game memory size (not that much hopefully) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Armor")
		TSubclassOf<UGameplayEffect> EquippedGameplayEffect;



	/*********************/
	/* Utility Funcitons */
	/*********************/
	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAttributeNamesAndValuesFromGameplayEffect(TArray<FAttributeDataUIStruct>& OutDataArray);

};
