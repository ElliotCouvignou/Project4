// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/GameplayEffects/EquipWeaponGameplayEffect.h"

UEquipWeaponGameplayEffect::UEquipWeaponGameplayEffect(const class FObjectInitializer& ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	
	
	// Create attribute's to affect by set-bycaller
	Modifiers.Push(FGameplayModifierInfo());

}



FGameplayModifierInfo UEquipWeaponGameplayEffect::MakeModifierInfoWithAttribute(FGameplayAttribute& Attribute, FGameplayTag& MagnitudeDataTag)
{
	FGameplayModifierInfo RetModifier = FGameplayModifierInfo();
	RetModifier.Attribute = Attribute;

	FSetByCallerFloat SBCF = FSetByCallerFloat();
	SBCF.DataTag = MagnitudeDataTag;
	RetModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SBCF);


	return RetModifier;
}