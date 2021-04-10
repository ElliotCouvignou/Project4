// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/ItemArmorDataAsset.h"
#include "P4ItemToolTip.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4ItemToolTip : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAttributeNamesAndValuesFromGameplayEffectClass(TArray<FAttributeDataUIStruct>& OutDataArray, TSubclassOf<UGameplayEffect> GEClass);

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAttributeNamesAndValuesFromItemInfo(const FItemBaseInfoStruct& ItemInfo, TArray<FAttributeDataUIStruct>& OutDataArray);

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAttributeNamesAndValuesFromGameplayEffectSpec(const FGameplayEffectSpec& GESpec, TArray<FAttributeDataUIStruct>& OutDataArray);
};
