// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4AbilitySystemComponent.h"
//#include "GameplayEffectTypes.h"




void UP4AbilitySystemComponent::BroadcastAutoAttackResults(const TArray<AProject4Character*>& HitArray)
{
	// Check size , if 0 then no hit -> no broadcast
	// If != 0 then broadcast both delegates 
	if (HitArray.Num() != 0)
	{
		AutoAttackHitCallback.Broadcast();
		AutoAttackHitTargetsCallback.Broadcast(HitArray);
	}
}

void UP4AbilitySystemComponent::FindAbilityLevelFromClass(TSubclassOf<UGameplayAbility> InAbilityClass, int& Level)
{
	FGameplayAbilitySpec* FoundSpecPtr = FindAbilitySpecFromClass(InAbilityClass);
	if (FoundSpecPtr)
	{
		Level = FoundSpecPtr->Level;
	}
	else
	{
		Level = 0;
	}
}

float UP4AbilitySystemComponent::PlayMontage(UGameplayAbility* AnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* Montage, float InPlayRate, FName StartSectionName, float StartTimeSeconds)
{
	float ret = Super::PlayMontage(AnimatingAbility, ActivationInfo, Montage, InPlayRate, StartSectionName);
	return ret;
}

bool UP4AbilitySystemComponent::SetGameplayEffectDurationWithHandle(const FActiveGameplayEffectHandle& Handle, float NewDuration)
{
	if (!Handle.IsValid())
	{
		return false;
	}

	const FActiveGameplayEffect* ActiveGameplayEffect = GetActiveGameplayEffect(Handle);
	if (!ActiveGameplayEffect)
	{
		return false;
	}

	FActiveGameplayEffect* AGE = const_cast<FActiveGameplayEffect*>(ActiveGameplayEffect);
	if (NewDuration > 0)
	{
		AGE->Spec.Duration = NewDuration;
	}
	else
	{
		AGE->Spec.Duration = 0.01f;
	}

	AGE->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime();
	AGE->CachedStartServerWorldTime = AGE->StartServerWorldTime;
	AGE->StartWorldTime = ActiveGameplayEffects.GetWorldTime();
	ActiveGameplayEffects.MarkItemDirty(*AGE);
	ActiveGameplayEffects.CheckDuration(Handle);

	AGE->EventSet.OnTimeChanged.Broadcast(AGE->Handle, AGE->StartWorldTime, AGE->GetDuration());
	OnGameplayEffectDurationChange(*AGE);

	return true;
}
