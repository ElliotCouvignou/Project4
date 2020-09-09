// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecutionCalculations/DamageExecution.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"



struct DamageAttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);

	
	// Meta Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DamageAttStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CritDamage, Source, false);

		// Meta Attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Damage, Target, false);
	}
};

DamageAttStruct& Damage()
{
	static DamageAttStruct It;
	return It;
}


UDamageExecution::UDamageExecution(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	DamageAttStruct Attributes;

	RelevantAttributesToCapture.Add(Attributes.DamageDef);

    //However, an attribute added here on top of being added in RelevantAttributesToCapture will still be captured, but will not be shown for potential in-function modifiers in the GameplayEffect blueprint, more on that later.
   // InvalidScopedModifierAttributes.Add(Attributes.HealthDef); 
	RelevantAttributesToCapture.Add(Attributes.CritChanceDef);
	RelevantAttributesToCapture.Add(Attributes.CritDamageDef);

}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    //Creating the attribute struct, we will need its values later when we want to get the attribute values.
	DamageAttStruct Attributes;

    // We put AbilitySystemComponents into little helper variables. Not necessary, 
    // but it helps keeping us from typing so much.
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr; //If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing by trying to get the AvatarActor variable from
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr; //a null pointer.

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Some more helper variables: Spec is the spec this execution originated from,
	// and the Source/TargetTags are pointers to the tags granted to source/target actor, respectively.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags(); 

	// We use these tags to set up an FAggregatorEvaluateParameters struct, 
	// which we will need to get the values of our captured attributes later in this function.
	FAggregatorEvaluateParameters EvaluationParameters; 
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float CritChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.CritChanceDef, EvaluationParameters, CritChance);

	float CritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.CritDamageDef, EvaluationParameters, CritDamage);

	/* Read input damage def value set by caller (not all do this but some due to context) */
	float InputDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.DamageDef, EvaluationParameters, InputDamage);


	//Finally, we go through our simple example damage calculation. Read from damage data tag (not all do this but some due to context)
	float BaseDamage = InputDamage + FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f); 
	
	float RawDamage = BaseDamage; // Apply Bonuses here

	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

	// Determine Crit Strike (uses applied bonuses)
	if (FMath::FRand() <= CritChance && !AssetTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Effect.CannotCrit"))))
	{
		RawDamage *= 1.f + CritDamage;

		/* Add dynamic crit tag to this GE spec To be read in player Attributes post exec */
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		MutableSpec->DynamicAssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Crit")));
		MutableSpec->DynamicAssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.True")));
	}

	// Relay damage val to meta attribute, will be handled by attribute class
	if (RawDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Damage().DamageProperty, EGameplayModOp::Additive, RawDamage));
		//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Damage().HealthProperty, EGameplayModOp::Additive, -DamageDone));
	}
	
	//Congratulations, your damage calculation is complete!
}
