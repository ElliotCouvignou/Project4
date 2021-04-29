// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/ModMagnitudeCalculations/P4AbilityModifierModMagCalc.h"
#include "AbilitySystem/AttributeSets/P4BaseAttributeSet.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"

UP4AbilityModifierModMagCalc::UP4AbilityModifierModMagCalc()
{

	//ManaDef defined in header FGameplayEffectAttributeCaptureDefinition ManaDef;
	//ManaDef.AttributeToCapture = UP4BaseAttributeSet::GetManaAttribute();
	//ManaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//ManaDef.bSnapshot = false;
	//
	////MaxManaDef defined in header FGameplayEffectAttributeCaptureDefinition MaxManaDef;
	//ManaMaxDef.AttributeToCapture = UP4BaseAttributeSet::GetManaMaxAttribute();
	//ManaMaxDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//ManaMaxDef.bSnapshot = false;
	//
	//RelevantAttributesToCapture.Add(ManaDef);
	//RelevantAttributesToCapture.Add(ManaMaxDef);
}

float UP4AbilityModifierModMagCalc::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather the tags from the source and target as that can affect which buffs should be used
	//const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	//const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//FAggregatorEvaluateParameters EvaluationParameters;
	//EvaluationParameters.SourceTags = SourceTags;
	//EvaluationParameters.TargetTags = TargetTags;


	UP4PlayerAbilitySystemComponent* SourceASC = Cast<UP4PlayerAbilitySystemComponent>(Spec.GetEffectContext().GetInstigatorAbilitySystemComponent());

	if(SourceASC)
	{
	
	}

	return 0.f;
}