// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Project4Controller.h"
#include "Characters/Project4Character.h"
#include "GameplayTagContainer.h"
#include "UI/GameplayHudWidget.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


UP4GameplayAbility::UP4GameplayAbility()
{
	// Instancing is necessary for ability costs and doesnt hurt to keep it on for all abilities
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Default tags that block this ability from activating 
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Buffs.Negative.Stunned")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Buffs.Negative.Silenced")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.DisabledAbilities")));
	
}

void UP4GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted) {
		// this will do something but returns a bool so just set it here and not care atm
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}


void UP4GameplayAbility::SendErrorMessageToUI(EAbilityErrorText ErrorType)
{
	// Get PC and send error to mainhud
	if (IsLocallyControlled()) {
		AProject4Controller* PController =
			Cast<AProject4Controller>(GetWorld()->GetFirstPlayerController());
		if (PController)
		{
			PController->SendUIAbilityError(ErrorType);
		}
	}
}

void UP4GameplayAbility::SendTargetDataToServer_Implementation(UP4GameplayAbility* AbilityRef, const FVector& HitLocation)
{
	//FGameplayAbilityTargetDataHandle TargetRef = FGameplayAbilityTargetDataHandle(DataHandle);
	AbilityRef->TargetDataLocation = FVector(HitLocation);
	print(FString(AbilityRef->TargetDataLocation.ToString()));
}



