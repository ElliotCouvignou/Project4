// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "EquipWeaponGameplayEffect.generated.h"

/**
 * c++ gameplayeffect for equipping weapons, this will set the attack interval appropriately 
 */
UCLASS()
class PROJECT4_API UEquipWeaponGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UEquipWeaponGameplayEffect(const class FObjectInitializer& ObjectInitializer);
	

protected:
	FGameplayModifierInfo MakeModifierInfoWithAttribute(FGameplayAttribute& Attribute, FGameplayTag& MagnitudeDataTag);

};
