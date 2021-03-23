// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Project4Controller.h"
#include "Characters/Project4Character.h"
#include "UI/AbilityTooltipWidget.h"
#include "GameplayTagContainer.h"
#include "UI/GameplayHudWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

FRotator UP4GameplayAbility::GetLookatRotation(float Range, FVector SourceLocation)
{
	// TODO: maybe expand this for case of AI casting spells to reuse spells
	FVector OutLocation;
	FRotator OutRotation;
	Cast<AProject4Controller>(GetActorInfo().PlayerController)->GetPlayerViewPoint(OutLocation, OutRotation);

	FRotator ShootRotation = UKismetMathLibrary::FindLookAtRotation(SourceLocation, UKismetMathLibrary::GetForwardVector(OutRotation) * Range + OutLocation);

	// try Hit test for floor to see if we should reduce rannge for angle changes (e.g aiming at floor offsets angle)
	FHitResult Result;
	FCollisionQueryParams CollisionParam;
	CollisionParam.AddIgnoredActor(GetOwningActorFromActorInfo());
	//DrawDebugLine(GetWorld(), OutLocation, OutLocation + UKismetMathLibrary::GetForwardVector(OutRotation) * Range, FColor::Green, true, 2.f, false, 4.f);
	GetWorld()->LineTraceSingleByProfile(Result, OutLocation, OutLocation + UKismetMathLibrary::GetForwardVector(OutRotation)*Range, FName("TargetActorGroundLocation"), CollisionParam);

	//if (Result.bBlockingHit)
	//{
	//	print("Blocking Hit!");
	//	DrawDebugLine(GetWorld(), SourceLocation, Result.Location, FColor::Red, true, 2.f, false, 4.f);
	//}

	return (Result.bBlockingHit) ? UKismetMathLibrary::FindLookAtRotation(SourceLocation, Result.Location) : ShootRotation;
}


