// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/GameplayEffects/P4AbilityModifierGameplayEffect.h"

UP4AbilityModifierGameplayEffect::UP4AbilityModifierGameplayEffect(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
}
