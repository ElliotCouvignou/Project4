// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/ExecutionCalculations/MagicDamageExecCalc.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"


struct FMagicDamageAttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);

	// Meta Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FMagicDamageAttStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CritDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, MagicResistance, Target, false);

		// Meta Attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Damage, Target, false);
	}
};

FMagicDamageAttStruct& MagicDamage()
{
	static FMagicDamageAttStruct It;
	return It;
}


UMagicDamageExecCalc::UMagicDamageExecCalc(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FMagicDamageAttStruct Attributes;

	RelevantAttributesToCapture.Add(Attributes.DamageDef);

	//However, an attribute added here on top of being added in RelevantAttributesToCapture will still be captured, but will not be shown for potential in-function modifiers in the GameplayEffect blueprint, more on that later.
   // 
	RelevantAttributesToCapture.Add(Attributes.CritChanceDef);
	//InvalidScopedModifierAttributes.Add(Attributes.CritChanceDef);
	RelevantAttributesToCapture.Add(Attributes.CritDamageDef);
	//InvalidScopedModifierAttributes.Add(Attributes.CritDamageDef);
	RelevantAttributesToCapture.Add(Attributes.MagicResistanceDef);
	//InvalidScopedModifierAttributes.Add(Attributes.MagicResistanceDef);

}

void UMagicDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Creating the attribute struct, we will need its values later when we want to get the attribute values.
	FMagicDamageAttStruct Attributes;

	// We put AbilitySystemComponents into little helper variables. Not necessary, 
	// but it helps keeping us from typing so much.
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr; //If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing by trying to get the AvatarActor variable from
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr; //a null pointer.

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

	float MagicResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.MagicResistanceDef, EvaluationParameters, MagicResistance);

	float InputDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.DamageDef, EvaluationParameters, InputDamage);

	// Crit chance bonus (additive)
	CritChance += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Attribute.CritChance.Bonus")), false, -1.0f), 0.f);
	if (CritChance > 1.f)
	{
		CritDamage += CritChance - 1.f;
	}


	// Crit damage effectiveness (default to 1.0)
	CritDamage *= FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Attribute.CritDamage.Effectiveness")), false, -1.0f), 1.0f);


	//Finally, we go through our simple example damage calculation. DefensePower comes from target.
	float BaseDamage = InputDamage + FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float RawDamage = BaseDamage; // Apply Bonuses here

	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

	FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
	// Determine Crit Strike (uses applied bonuses)
	if (FMath::FRand() <= CritChance && !AssetTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Effect.CannotCrit"))))
	{
		RawDamage *= 1.f + CritDamage;

		/* Add dynamic Tags to this GE for damage info */
		
		MutableSpec->DynamicAssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Crit")));
		
	}

	float DamageDone = RawDamage * (200.f / (200.f + MagicResistance)); // Apply mitigation here; // Apply mitigation here

	// Relay damage val to meta attribute, will be handled by attribute class
	if (DamageDone > 0.f)
	{
		MutableSpec->DynamicAssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Magic")));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(MagicDamage().DamageProperty, EGameplayModOp::Additive, DamageDone));
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	}

	//Congratulations, your damage calculation is complete!
}
