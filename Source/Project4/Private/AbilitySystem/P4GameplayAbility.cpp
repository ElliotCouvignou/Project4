// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Project4Controller.h"
#include "Characters/Project4Character.h"
#include "Project4Controller.h"
#include "UI/AbilityTooltipWidget.h"
#include "GameplayTagContainer.h"
#include "UI/GameplayHudWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
//#include "Intersection/IntersectionUtil.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)
#define CONSTRUCT_CLASS(Class, Name) Name = NewObject<Class>(GetTransientPackage(), FName(TEXT(#Name)))

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

void UP4GameplayAbility::CreateCustomGameplayEffectSpec(TSubclassOf<UGameplayEffect> EffectClass, const FP4GEExposedParametersStruct& Params, const int& Level, FGameplayEffectSpecHandle& Result)
{
	// Create New GE or reuse existing object (neeeds same obj ref for stacking)

	//for (auto e : GeneratedGameplayEffects)
	//{
	//	if (e.Params == Params && EffectClass == e.BaseClass)
	//	{
	//		//GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	//
	//		UGameplayEffect* GE = e.GeneratedObject->CreateDefaultSubobject<UGameplayEffect>(FName(EffectClass.Get()->GetName() + "_Generated"));
	//		
	//		FGameplayEffectContextHandle Context = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
	//		if (!Context.IsValid())
	//		{
	//			Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	//		}
	//		
	//		FGameplayEffectSpec* NewSpec = new FGameplayEffectSpec(GE, Context, Level);
	//		Result = FGameplayEffectSpecHandle(NewSpec);
	//		return;
	//
	//		//FGameplayEffectSpec* NewSpec = new FGameplayEffectSpec(e.GeneratedSpec);
	//		//NewSpec->SetLevel(Level);
	//		//Result = FGameplayEffectSpecHandle(NewSpec);
	//		//print(FString("Found old"));	
	//		//return;
	//	}
	//}

	//CONSTRUCT_CLASS(UGameplayEffect, GE);
	//UGameplayEffect* GE = NewObject<UGameplayEffect>(EffectClass->GetDefaultObject());

	UGameplayEffect* GE = EffectClass->GetDefaultObject<UGameplayEffect>();

	// Construct new Custom GE
	if ((Params.Stacking.Key))
	{
		GE->StackLimitCount = Params.Stacking.StackLimitCount;
		GE->StackingType = Params.Stacking.StackingType;
		GE->StackDurationRefreshPolicy = Params.Stacking.StackDurationRefreshPolicy;
		GE->StackPeriodResetPolicy = Params.Stacking.StackPeriodResetPolicy;
		GE->StackExpirationPolicy = Params.Stacking.StackExpirationPolicy;
	}

	// TODO: make sure this doesnt cause mem leaks 
	FGameplayEffectContextHandle Context = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
	if (!Context.IsValid())
	{
		Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	}

	//GetAbilitySystemComponentFromActorInfo()->GetHandle;
	FGameplayEffectSpec* NewSpec = new FGameplayEffectSpec(GE, Context, Level);
	Result = FGameplayEffectSpecHandle(NewSpec);

	FP4GECustomGameplayEffectStruct Genstruct;
	Genstruct.Params = Params;
	Genstruct.BaseClass = EffectClass;
	Genstruct.GeneratedObject = GE;
	Genstruct.GeneratedSpec = *NewSpec;
//	GeneratedGameplayEffects.AddUnique(Genstruct);
}

//void UP4GameplayAbility::SetContitionalGameplayEffectSetByCallerByTag(TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag CallerTag, float Value)
//{
//}

void UP4GameplayAbility::SendTargetDataToServer_Implementation(UP4GameplayAbility* AbilityRef, const FVector& HitLocation)
{
	//FGameplayAbilityTargetDataHandle TargetRef = FGameplayAbilityTargetDataHandle(DataHandle);
	AbilityRef->TargetDataLocation = FVector(HitLocation);
	print(FString(AbilityRef->TargetDataLocation.ToString()));
}

FRotator UP4GameplayAbility::GetLookatRotation(float Range, FVector SourceLocation)
{
	// first test is to try to aim towards ground location if player is aiming down that much. if no blocking hit then 
	// create a line, project our resulting angle in such a way that the line from the source locaiton
	// 	   to the created line has a distance of Range
	// 
	// TODO: maybe expand this for case of AI casting spells to reuse spells
	FVector OutLocation;
	FRotator OutRotationR;
	Cast<AProject4Controller>(GetActorInfo().PlayerController)->GetPlayerViewPoint(OutLocation, OutRotationR);
	FVector OutRotation = OutRotationR.Vector();

	// try Hit test for floor to see if we should reduce rannge for angle changes (e.g aiming at floor offsets angle)
	FHitResult Result;
	FCollisionQueryParams CollisionParam;
	CollisionParam.AddIgnoredActor(GetOwningActorFromActorInfo());
	//DrawDebugLine(GetWorld(), OutLocation, OutLocation + UKismetMathLibrary::GetForwardVector(OutRotation) * Range, FColor::Green, true, 2.f, false, 4.f);
	GetWorld()->LineTraceSingleByProfile(Result, OutLocation, OutLocation + OutRotation * Range, FName("AbilityProjectile"), CollisionParam);
	if (Result.bBlockingHit)
	{
		//DrawDebugLine(GetWorld(), OutLocation, OutLocation + OutRotation * Range, FColor::Green, true, 2.f, false, 4.f);
		//DrawDebugLine(GetWorld(), SourceLocation, Result.Location, FColor::Red, true, 2.f, false, 4.f);
		//DrawDebugPoint(GetWorld(), Result.Location, 10.f, FColor::Green, true, 2.f, false);
		//DrawDebugPoint(GetWorld(), SourceLocation, 10.f, FColor::Green, true, 2.f, false);
		//print(FString("1 LookatLocation: " + Result.Location.ToString()));

		// raise a little just in case since they're generally looking downwards (can remove)
		return UKismetMathLibrary::FindLookAtRotation(SourceLocation, Result.Location);
	}

	// implementaiton taken from IntersectionUtil::RaySphereTest and RaySphereIntersection
	FVector diff = OutLocation - SourceLocation;
	float a0 = diff.SizeSquared() - Range * Range;
	float a1 = FVector::DotProduct(diff, OutRotation);
	float discr = a1 * a1 - a0;
	
	FVector LookatLocation = OutLocation;
	if (discr > 0)
	{
		// Two hits, this happends when camera starts outside range sphere, the intersection to choose is the one farthest from outlocation
		//ResultOut.parameter.Min = -a1 - root;
		//ResultOut.parameter.Max = -a1 + root;
		float root = FMath::Sqrt(discr);
		//print(FString("discr > 0  Dist: " + FString::SanitizeFloat((-a1) + root, 2)));
		LookatLocation = OutLocation + OutRotation * ((-a1) + root);
	}
	else if (discr < 0)
	{
		//print(FString("Increase range for this ability i cant find the UP4GameplayAbility::GetLookatRotation result like this"));
	}
	else 
	{
		// -a1 is distance along line that we hit
		//print(FString("!(discr < 0)"));
		LookatLocation = OutLocation + OutRotation *(-a1);
	}
	//DrawDebugLine(GetWorld(), OutLocation, OutLocation + OutRotation * Range, FColor::Green, true, 2.f, false, 4.f);
	//DrawDebugLine(GetWorld(), SourceLocation, LookatLocation, FColor::Red, true, 2.f, false, 4.f);
	//DrawDebugPoint(GetWorld(), LookatLocation, 10.f, FColor::Green, true, 2.f, false);
	//DrawDebugPoint(GetWorld(), SourceLocation, 10.f, FColor::Green, true, 2.f, false);
	//print(FString("2 LookatLocation: " + LookatLocation.ToString()));
	return UKismetMathLibrary::FindLookAtRotation(SourceLocation, LookatLocation);

	
	
	//FRotator ShootRotation = UKismetMathLibrary::FindLookAtRotation(SourceLocation, UKismetMathLibrary::GetForwardVector(OutRotation) * Range + OutLocation);


	//if (Result.bBlockingHit)
	//{
	//	print("Blocking Hit!");
	//	DrawDebugLine(GetWorld(), SourceLocation, Result.Location, FColor::Red, true, 2.f, false, 4.f);
	//}
	//return (Result.bBlockingHit) ? UKismetMathLibrary::FindLookAtRotation(SourceLocation, Result.Location) : ShootRotation;
}

//void UP4GameplayAbility::CalculateAbilityMagnitudes_Implementation(const int& AbilityLevel, TArray<float>& Results)
//{
//	Results.Empty();
//}


