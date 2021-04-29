// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project4Controller.h"
#include "P4FloatingTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4FloatingTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetDamageText(FP4DamageNumber Damage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealText(FP4DamageNumber Heal);
};
