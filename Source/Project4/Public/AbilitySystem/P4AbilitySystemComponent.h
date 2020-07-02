// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "P4AbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRecievedDamageDelegate, UP4AbilitySystemComponent*, SourceASC, float, RawDamage, float, TotalDamage);

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;
	
	FRecievedDamageDelegate RecievedDamage;

	// called from DamageExecCalculation, to broadcast damage as delegate
	virtual void RecieveDamage(UP4AbilitySystemComponent* SourceASC, float RawDamage, float TotalDamage);
};
