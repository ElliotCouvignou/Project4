// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/ExecutionCalculations/EnrageExecutionCalculation.h"
#include "AbilitySystem/PlayerAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)



struct FEnrageAttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeed);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);

	FEnrageAttStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, AttackSpeed, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, CritDamage, Source, false);
	}
};

FEnrageAttStruct& EnrageStruct()
{
	static FEnrageAttStruct It;
	return It;
}

UEnrageExecutionCalculation::UEnrageExecutionCalculation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FEnrageAttStruct Attributes;

	RelevantAttributesToCapture.Add(Attributes.AttackSpeedDef);
	InvalidScopedModifierAttributes.Add(Attributes.AttackSpeedDef);
	RelevantAttributesToCapture.Add(Attributes.CritChanceDef);
	InvalidScopedModifierAttributes.Add(Attributes.CritChanceDef);
	RelevantAttributesToCapture.Add(Attributes.CritDamageDef);
	InvalidScopedModifierAttributes.Add(Attributes.CritDamageDef);
}

void UEnrageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Creating the attribute struct, we will need its values later when we want to get the attribute values.
	FEnrageAttStruct Attributes;

	// We put AbilitySystemComponents into little helper variables. Not necessary, 
	// but it helps keeping us from typing so much.
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->AvatarActor : nullptr; //If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing by trying to get the AvatarActor variable from
	AActor* TargetActor = TargetASC ? TargetASC->AvatarActor : nullptr; //a null pointer.

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

	print(FString("in EXEC calc"));

	// Get # of frenzy stacks
	FGameplayTag FrenzyTag = FGameplayTag::RequestGameplayTag(FName("Buffs.Positive.Frenzy"));
	TArray<FActiveGameplayEffectHandle> ActiveGEArray = SourceASC->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(FrenzyTag)));
	for (FActiveGameplayEffectHandle& GEHandle : ActiveGEArray)
	{
		// This should really only loop once 
		const FActiveGameplayEffect* ActiveGE = SourceASC->GetActiveGameplayEffect(GEHandle);
		float ASmag = ActiveGE->Spec.GetSetByCallerMagnitude(FName("Data.Buffs.Positive.FrenzyAS"));

		int32 StackCount = SourceASC->GetCurrentStackCount(GEHandle);

		float ASBonus = ASmag * StackCount;

		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(EnrageStruct().AttackSpeedProperty, EGameplayModOp::Additive, ASBonus));

		print(FString("BOnus = " + FString::SanitizeFloat(ASBonus, 2)));

		GEHandle.RemoveFromGlobalMap();
		
	}
	
}
