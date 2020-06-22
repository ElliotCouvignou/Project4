// Fill out your copyright notice in the Description page of Project Settings.


#include "P4GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"


UP4GameplayAbility::UP4GameplayAbility()
{
	// Instancing is necessary for ability costs and doesnt hurt to keep it on for all abilities
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Default tags that block this ability from activating 
	//ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	//ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}


void UP4GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted) {
		// this will do something but returns a bool so just set it here and not care atm
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UP4GameplayAbility::SetAbilityInputID(EP4AbilityInputID NewInputID)
{
	AbilityID = NewInputID;
}

