// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecution.h"
#include "Project4.h"

struct AttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health); //The DECLARE_ATTRIBUTE_CAPTUREDEF macro actually only declares two variables. The variable names are dependent on the input, however. Here they will be HealthProperty(which is a UPROPERTY pointer)
										  //and HealthDef(which is a FGameplayEffectAttributeCaptureDefinition).
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);

	AttStruct()
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
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, MagicPower, Source, true);

		//The same rules apply for the multiplier attributes too.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, MagicResistance, Target, false);
	}
};

AttStruct& Damage()
{
	static AttStruct It;
	return It;
}


UDamageExecution::UDamageExecution(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    AttStruct Attributes;

    //RelevantAttributesToCapture is the array that contains all attributes you wish to capture, without exceptions. 
    RelevantAttributesToCapture.Add(Attributes.HealthDef);  
    //However, an attribute added here on top of being added in RelevantAttributesToCapture will still be captured, but will not be shown for potential in-function modifiers in the GameplayEffect blueprint, more on that later.
    InvalidScopedModifierAttributes.Add(Attributes.HealthDef); 

    RelevantAttributesToCapture.Add(Attributes.AttackPowerDef);
    RelevantAttributesToCapture.Add(Attributes.MagicPowerDef);
    
    RelevantAttributesToCapture.Add(Attributes.ArmorDef);
    RelevantAttributesToCapture.Add(Attributes.MagicResistanceDef);
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    //Creating the attribute struct, we will need its values later when we want to get the attribute values.
    AttStruct Attributes;

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

	float Health = 0.f;
	// Alright, this is where we get the attribute's captured value into our function. 
	// Damage().HealthDef is the definition of the attribute we want to get, we defined EvaluationParameters 
	// just above us, and Health is the variable where we will put the captured value into(the Health variable we just declared)
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HealthDef, EvaluationParameters, Health);

	// We do this for all other attributes, as well.
	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.AttackPowerDef, EvaluationParameters, AttackPower);

	float MagicPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.MagicPowerDef, EvaluationParameters, MagicPower);

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.ArmorDef, EvaluationParameters, Armor);

	float MagicResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.MagicResistanceDef, EvaluationParameters, MagicResistance);

	

	//Finally, we go through our simple example damage calculation. BaseAttackPower and AttackMultiplier come from soruce, DefensePower comes from target.
	float DamageDone = AttackPower / Armor + MagicPower / MagicResistance;

	print(FString::SanitizeFloat(DamageDone, 3));
		
	// An optional step is to clamp to not take health lower than 0. This can be ignored,
	// or implemented in the attribute sets' PostGameplayEffectExecution function. Your call, really.
	// DamageDone = FMath::Min<float>(Damage, Health);

	// Finally, we check if we even did any damage in this whole ordeal. 
	// If yes, then we will add an outgoing execution modifer to the Health attribute we got from our target, 
	// which is a modifier that can still be thrown out by the attribute system if it wishes to throw out the 
	// GameplayEffectExecutionCalculation.
	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Damage().HealthProperty, EGameplayModOp::Additive, -DamageDone));
	}

	//Congratulations, your damage calculation is complete!
}
