// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/ExecutionCalculations/HealingCalculation.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"

struct HealAttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health); //The DECLARE_ATTRIBUTE_CAPTUREDEF macro actually only declares two variables. The variable names are dependent on the input, however. Here they will be HealthProperty(which is a UPROPERTY pointer)
										  //and HealthDef(which is a FGameplayEffectAttributeCaptureDefinition).

	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);

	// Meta Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Heal);

	HealAttStruct()
	{
		// We define the values of the variables we declared now. In this example, 
		//HealthProperty will point to the Health attribute in the UPlayerAttributeSet 
		//on the receiving target of this execution. The last parameter is a bool, 
		//and determines if we snapshot the attribute's value at the time of definition.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Health, Target, false);

		//This here is a different example: We still take the attribute from UPlayerAttributeSet, 
		//but this time it is BaseAttackPower, and we look at the effect's source for it. 
		//We also want to snapshot is because the effect's strength should be determined during
		//its initial creation. A projectile wouldn't change damage values depending on the 
		// source's stat changes halfway through flight, after all.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, MagicPower, Source, true);

		// Meta Attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Heal, Target, false);
	}
};

HealAttStruct& Heal()
{
	static HealAttStruct It;
	return It;
}


UHealingCalculation::UHealingCalculation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HealAttStruct Attributes;

	//RelevantAttributesToCapture is the array that contains all attributes you wish to capture, without exceptions. 
	RelevantAttributesToCapture.Add(Attributes.HealthDef);
	//However, an attribute added here on top of being added in RelevantAttributesToCapture will still be captured, but will not be shown for potential in-function modifiers in the GameplayEffect blueprint, more on that later.
	//InvalidScopedModifierAttributes.Add(Attributes.HealthDef);

	RelevantAttributesToCapture.Add(Attributes.MagicPowerDef);
}

void UHealingCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Creating the attribute struct, we will need its values later when we want to get the attribute values.
	HealAttStruct Attributes;

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

	// Alright, this is where we get the attribute's captured value into our function. 
	// Damage().HealthDef is the definition of the attribute we want to get, we defined EvaluationParameters 
	// just above us, and Health is the variable where we will put the captured value into(the Health variable we just declared)
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HealthDef, EvaluationParameters, Health);

	float MagicPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.MagicPowerDef, EvaluationParameters, MagicPower);


	//Finally, we go through our simple example damage calculation. BaseAttackPower and AttackMultiplier come from soruce, DefensePower comes from target.
	float BaseHeal = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Heal")), false, -1.0f), 0.0f); //AttackPower / Armor + MagicPower / MagicResistance;

	float RawHeal = BaseHeal; // Apply Bonuses here

	float HealingDone = RawHeal; // Apply mitigation here


	// Finally, we check if we even did any damage in this whole ordeal. 
	// If yes, then we will add an outgoing execution modifer to the Health attribute we got from our target, 
	// which is a modifier that can still be thrown out by the attribute system if it wishes to throw out the 
	// GameplayEffectExecutionCalculation.
	if (HealingDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Heal().HealProperty, EGameplayModOp::Additive, HealingDone));
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	}
}
