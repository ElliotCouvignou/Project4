// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/ExecutionCalculations/LacerationExecutionCalculation.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)



struct FLacerationAttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Mana);

	FLacerationAttStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Mana, Source, false);
	}
};

FLacerationAttStruct& LacerateStruct()
{
	static FLacerationAttStruct It;
	return It;
}

ULacerationExecutionCalculation::ULacerationExecutionCalculation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FLacerationAttStruct Attributes;

	RelevantAttributesToCapture.Add(Attributes.ManaDef);
	//InvalidScopedModifierAttributes.Add(Attributes.ManaDef); 
}

void ULacerationExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Creating the attribute struct, we will need its values later when we want to get the attribute values.
	FLacerationAttStruct Attributes;

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


	if (SourceActor && TargetActor && (SourceActor->GetDistanceTo(TargetActor) <= RefundDistance) )
	{
		// Refund Mana
		SourceAbilitySystemComponent->ApplyModToAttribute(Attributes.ManaProperty, EGameplayModOp::Additive, ManaRefundAmount);
		
		// find CD tag, get duration and reapply with refunded cd
		FGameplayTag LacerationCDTag = FGameplayTag::RequestGameplayTag(FName("Cooldown.Skill.Laceration"));
		TArray<FActiveGameplayEffectHandle> CD_GEArray = SourceAbilitySystemComponent->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(LacerationCDTag)));
		for (const FActiveGameplayEffectHandle& CD_GEHandle : CD_GEArray)
		{
			const FActiveGameplayEffect* ActiveGE = SourceAbilitySystemComponent->GetActiveGameplayEffect(CD_GEHandle);	
			UWorld* World = SourceActor->GetWorld();
			if (World)
			{
				float NewCDDuration = FMath::Clamp(ActiveGE->GetTimeRemaining(World->GetTimeSeconds()) - CooldownRefundAmount, 0.f, 9000.f);
				UP4AbilitySystemComponent* P4ASC = Cast<UP4AbilitySystemComponent>(SourceAbilitySystemComponent);
				if (P4ASC)
				{
					// Finally Call CD reduction
					//print(FString("Set to: " + FString::SanitizeFloat(NewCDDuration, 2)));
					P4ASC->SetGameplayEffectDurationWithHandle(CD_GEHandle, NewCDDuration);
				}
			}
		}
	}
}
