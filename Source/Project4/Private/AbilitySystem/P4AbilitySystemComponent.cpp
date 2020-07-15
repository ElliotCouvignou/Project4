// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4AbilitySystemComponent.h"


float UP4AbilitySystemComponent::PlayMontage(UGameplayAbility* AnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* Montage, float InPlayRate, FName StartSectionName)
{
	float ret = Super::PlayMontage(AnimatingAbility, ActivationInfo, Montage, InPlayRate, StartSectionName);
	return ret;
}

void UP4AbilitySystemComponent::RecieveDamage(UP4AbilitySystemComponent* SourceASC, float RawDamage, float TotalDamage)
{
	RecievedDamage.Broadcast(SourceASC, RawDamage, TotalDamage);
}
