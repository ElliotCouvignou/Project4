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


	/* Virtual Overrides */

	/* Calls palyMontage ASC function that isn't blueprint exposed on default
		Similar function exists in P4GameplayAbility with a target actor */
	UFUNCTION(BlueprintCallable)
		virtual float PlayMontage(UGameplayAbility* AnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* Montage, float InPlayRate, FName StartSectionName = NAME_None) override;

	// called from DamageExecCalculation, to broadcast damage as delegate
	virtual void RecieveDamage(UP4AbilitySystemComponent* SourceASC, float RawDamage, float TotalDamage);
};
