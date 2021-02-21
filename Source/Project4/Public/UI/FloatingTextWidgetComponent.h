// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Project4Controller.h"
#include "FloatingTextWidgetComponent.generated.h"

/**
 *  This class is responsible for displaying numbers from abilities
 *  An Example is the damage text and healing numbers on affected targets
 */
UCLASS()
class PROJECT4_API UFloatingTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetDamageText(FP4DamageNumber Damage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealText(float Heal);

	// Others might come up (e.g Buff/Debuff Notifs)


};
