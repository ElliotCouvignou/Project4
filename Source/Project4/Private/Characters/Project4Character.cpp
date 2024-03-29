// Copyright Epic Games, Inc. All Rights Reserved.
// define a print message function to print to screen

#include "Characters/Project4Character.h"
#include "Project4Controller.h"
#include "Project4.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Containers/Array.h"

#include "UI/GameplayHudWidget.h"
#include "UI/FloatingStatusBarWidget.h"
#include "UI/P4FloatingTextWidget.h"

#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "AbilitySystem/P4GameplayAbilitySet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"

#include "Characters/P4CharacterMovementComponent.h"
#include "AI/P4AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"




#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


//////////////////////////////////////////////////////////////////////////
// AProject4Character

AProject4Character::AProject4Character(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UP4CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Networking characteristics for characters
	bAlwaysRelevant = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1024.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;

	/* add RH and LH weapon Skeletal meshes socketed to hands */
	MeshLH = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshLH"));
	MeshLH->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("Hand_L_Socket")));
	MeshLH->SetIsReplicated(true);

	MeshRH = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshRH"));
	MeshRH->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("hand_r_Socket")));
	MeshRH->SetIsReplicated(true);

	UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIFloatingStatusBarComponent"));
	UIFloatingStatusBarComponent->bWantsInitializeComponent = true;
	UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
	UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500, 500));

	FloatingTextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("FloatingTextWidgetComponent"));
	FloatingTextWidgetComponent->bWantsInitializeComponent = true;
	FloatingTextWidgetComponent->SetupAttachment(GetMesh());
	FloatingTextWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingTextWidgetComponent->SetDrawSize(FVector2D(500, 500));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	AliveTag = FGameplayTag::RequestGameplayTag(FName("State.Alive"));
	StunnedTag = FGameplayTag::RequestGameplayTag(FName("Buffs.Negative.Stunned"));

	//RootComponent->SetVisibility(false, true);

	//Respawn = FGameplayTag::RequestGameplayTag(FName(""));
}

void AProject4Character::TryGetTarget(float Range, bool AllowEnemies, bool AllowAllies, AProject4Character*& Result)
{
	Result = nullptr;

	// Return SelectedTarget if Valid
	AProject4Character* P4SelectedTarget = Cast<AProject4Character>(SelectedTarget);
	if (P4SelectedTarget)
	{
		bool InFront = FVector::DotProduct(GetController()->GetDesiredRotation().Vector(), SelectedTarget->GetActorLocation() - GetActorLocation()) >= 0.f;
		bool ValidTargetType = (AllowEnemies && IsEnemiesWith(P4SelectedTarget)) || (AllowAllies && IsAlliesWith(P4SelectedTarget));
		float dist = P4SelectedTarget->GetDistanceTo(this);

		if (InFront && dist <= Range && ValidTargetType)
		{
			Result = P4SelectedTarget;
			return;
		}
	}


	// if AI return their blackboard stored target, not doing checks cause if we crash here then cmon bruh we got bigger problems
	AP4AIControllerBase* AIC = Cast<AP4AIControllerBase>(GetController());
	if (AIC)
	{
		Result = Cast<AProject4Character>(AIC->GetBlackboardComponent()->GetValueAsObject(FName("TargetActor")));
		return;
	}


	TryGetTargetInFront(Range, AllowEnemies, AllowAllies, Result);

	// made it here then no target
}

void AProject4Character::TryGetTargetInFront(float Range, bool AllowEnemies, bool AllowAllies, AProject4Character*& Result)
{
	// to ensure player precision while allowing forgiveness we cycle through multiple passes of cylinder linetraces at increasing radius
	AProject4Controller* P4C = Cast<AProject4Controller>(GetController());
	if (!P4C)
		return;

	// Gather General Trace start/end, same for all subsequent traces
	FVector Start; FRotator OutRot;
	P4C->GetPlayerViewPoint(Start, OutRot);
	FVector End = Start + 5250 * OutRot.Vector();

	// temp magic numbers as vars to be moved later 
	int Iterations = 4;
	float RadiusInc = 52.5;
	float Radius = 0.01f;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	for (int i = 0; i < Iterations; i++)
	{
		//Re-initialize hit info
		TArray<FHitResult> HitResult;
		bool Hit = UKismetSystemLibrary::SphereTraceMultiByProfile(
			GetWorld(),
			Start, End, Radius,
			FName("Targetable"),
			true,
			IgnoreActors,
			EDrawDebugTrace::ForOneFrame,
			HitResult,
			true,
			FLinearColor::Red, FLinearColor::Green, 0.1f
		);

		if (Hit && GetClosestTargetActor(HitResult, Range, AllowEnemies, AllowAllies, Result))
			return;

		Radius += RadiusInc;
	}

}

bool AProject4Character::GetClosestTargetActor(TArray<FHitResult>& Hits, float Range, bool AllowEnemies, bool AllowFriendlies, AProject4Character*& Result)
{
	float SmallestDist = 9999999999.f;
	Result = nullptr;
	bool ret = false;

	for (FHitResult e : Hits)
	{
		AProject4Character* P4Target = Cast<AProject4Character>(e.Actor);
		if (P4Target)
		{
			bool InFront = FVector::DotProduct(GetController()->GetDesiredRotation().Vector(), e.Actor->GetActorLocation() - GetActorLocation()) >= 0.f;
			bool ValidTargetType = (AllowEnemies && IsEnemiesWith(P4Target)) || (AllowFriendlies && IsAlliesWith(P4Target));
			float dist = P4Target->GetDistanceTo(this);
			if (InFront && dist <= Range && ValidTargetType && SmallestDist > dist)
			{
				Result = P4Target;
				SmallestDist = dist;
				ret = true;
			}
		}
	}

	return ret;
}



bool AProject4Character::IsEnemiesWith(AProject4Character* Other)
{
	return true;
}

bool AProject4Character::IsAlliesWith(AProject4Character* Other)
{
	return false;
}

UAbilitySystemComponent* AProject4Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}


UP4BaseAttributeSet* AProject4Character::GetAttributeSet() const
{
	return AttributeSet.Get();
}



bool AProject4Character::IsAlive() const
{
	if (AttributeSet.Get()) {
		return (AttributeSet->GetHealth() > 0.f);
	}
	return true;
}

void AProject4Character::Die()
{
	// Stop player and remove collisions, freeze in air
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCharacterMovement()->GravityScale = 0;
	//GetCharacterMovement()->Velocity = FVector(0);

	// broadcast delegate I ded
	OnCharacterDied.Broadcast(this);



	if (AbilitySystemComponent.IsValid())
	{
		// stop all abilitites regardless of tags (not GE's)
		AbilitySystemComponent->CancelAllAbilities();
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
		AbilitySystemComponent->SetTagMapCount(AliveTag, 0);

		// apply ondeath gameplay effects
		ApplyDeathGameplayEffects();
	}
	MulticastDeath();

}

void AProject4Character::FinishDying()
{
	//Destroy();
}

void AProject4Character::MulticastDeath_Implementation()
{
	OnCharacterDied.Broadcast(this);

	// hide floating hp bar
	UIFloatingStatusBarComponent->SetVisibility(false, true);

	// play death montage if set, else play ALS ragdoll with manual delay
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
		GetWorld()->GetTimerManager().SetTimer(RadgollDeathHandle, this, &AProject4Character::FinishDying, DeathMontage->GetPlayLength(), false);
	}
	else
	{
		// calls ALS to do ragdoll
		ActivateRagdoll();
		GetWorld()->GetTimerManager().SetTimer(RadgollDeathHandle, this, &AProject4Character::FinishDying, RadgollDeathDelay, false);

		GetCharacterMovement()->Deactivate();
		//SetReplicates(false);
	}
}

void AProject4Character::MulticastRespawn_Implementation()
{
	if (!DeathMontage)
	{
		// HACK: kindof assuming we are in ragdoll to undo it
		//SetReplicates(true);
		GetCharacterMovement()->Activate();

		ActivateRagdoll();
	}
}


void AProject4Character::Respawn_Implementation()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		AbilitySystemComponent->AddLooseGameplayTag(AliveTag);
		RemoveDeathGameplayEffects();
		ApplyRespawnGameplayEffects();
	}

	MulticastRespawn();
	
}

/***************************/
/*           UI            */
/***************************/

void AProject4Character::InitFloatingStatusBarWidget()
{
	if (AbilitySystemComponent.IsValid())
	{
		// Setup FloatingStatusBar UI for Locally Owned Players only but exclude your own FSB
		// Skip Init when no pc is present (NPC's dont need to see hp bars)
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->IsLocalPlayerController() && PC != GetController<APlayerController>())
		{
			if (UIFloatingStatusBarClass)
			{
				UIFloatingStatusBar = CreateWidget<UFloatingStatusBarWidget>(PC, UIFloatingStatusBarClass);
				if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
				{
					UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);
					UIFloatingStatusBarComponent->SetDrawSize(UIFloatingStatusBar->GetDesiredSize());
					UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()* FloatingStatusBarHeightCoeff));
					UIFloatingStatusBarComponent->SetCullDistance(FloatingStatusBarCullDistance);

					// Setup the floating status bar
					if (AttributeSet.IsValid())
					{
						UIFloatingStatusBar->SetHealthPercentage(AttributeSet->GetHealth() / AttributeSet->GetHealthMax());
						UIFloatingStatusBar->SetCharacterName(CharacterName);
						UIFloatingStatusBar->Owner = this;
					}
					
				}
			}
		}
	}
}

void AProject4Character::InitFloatingTextWidgetComponent()
{
	// idk why but owning palyer doesn't initialize sometimes so do this in case
	// Apparently this might be some BP and c++ updates not linking right long ago
	if (!FloatingTextWidgetComponent)
	{
		FloatingTextWidgetComponent = NewObject<UWidgetComponent>(this);
		FloatingTextWidgetComponent->RegisterComponent();
		FloatingTextWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		FloatingTextWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		FloatingTextWidgetComponent->SetDrawSize(FVector2D(500, 500));
	
	}

	if (AbilitySystemComponent.IsValid())
	{
		// Setup FloatingStatusBar UI for Locally Owned Players only but exclude your own FSB
		// Skip Init when no pc is present (NPC's dont need to see hp bars)
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			if (UIFloatingTextWidgetClass)
			{
				UIFloatingTextWidget = CreateWidget<UP4FloatingTextWidget>(PC, UIFloatingTextWidgetClass);
				if (UIFloatingTextWidget && FloatingTextWidgetComponent)
				{
					FloatingTextWidgetComponent->SetWidget(UIFloatingTextWidget);
					FloatingTextWidgetComponent->SetDrawSize(UIFloatingTextWidget->GetDesiredSize());
					//FloatingTextWidgetComponent->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * FloatingStatusBarHeightCoeff));
					FloatingTextWidgetComponent->SetCullDistance(FloatingStatusBarCullDistance);
				}
			}
		}
	}
}

UFloatingStatusBarWidget* AProject4Character::GetFloatingStatusBarWidget()
{
	return UIFloatingStatusBar;
}

UP4FloatingTextWidget* AProject4Character::GetFloatingTextWidget()
{
	return (UIFloatingTextWidget) ? UIFloatingTextWidget : Cast<UP4FloatingTextWidget>(FloatingTextWidgetComponent->GetWidget());
}

void AProject4Character::SetRightHandWeaponInfo(const UP4ItemWeaponObject* WeaponObject)
{
	if (HasAuthority())
	{
		if (WeaponObject)
		{
			MainHandWeaponType = WeaponObject->WeaponType;
			if (MainHandWeaponType == EWeaponType::Bow)
			{
				// unique case where you use your main hand to pull bow strings back, therefore attach to offhand
				MulticastSetWeaponSkeletalMesh(false, WeaponObject->WeaponSkeletalMesh, WeaponObject->MainHandAttatchTransform);
			}
			else
			{
				MulticastSetWeaponSkeletalMesh(true, WeaponObject->WeaponSkeletalMesh, WeaponObject->MainHandAttatchTransform);
			}
			
			// TODO: remake this wih new procedural generated GE
			// Apply Equip weapon GE for attack interval
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(EquipWeaponMainGameplayEffect, 1, EffectContext);
			
			// set by caller with weapon interval to correct attribute, leave other as is 
			if (NewHandle.IsValid())
			{
				FGameplayEffectSpec* GESpec = NewHandle.Data.Get();
				// set interval attribute
				FGameplayTag RightHandIntervalTag = FGameplayTag::RequestGameplayTag(FName("Data.Attribute.MainHandAttackInterval"));
				GESpec->SetSetByCallerMagnitude(RightHandIntervalTag, WeaponObject->AttackInterval);
				// set weapon power attribute
				FGameplayTag RightHandWeaponPowerTag = FGameplayTag::RequestGameplayTag(FName("Data.Attribute.MainHandWeaponPower"));
				GESpec->SetSetByCallerMagnitude(RightHandWeaponPowerTag, WeaponObject->WeaponPower);
			
			
				WeaponMainActiveGE = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
			}
		}
	}
}

void AProject4Character::SetLeftHandWeaponInfo(const UP4ItemWeaponObject* WeaponObject)
{
	if (HasAuthority())
	{
		if (WeaponObject)
		{
			OffHandWeaponType = WeaponObject->WeaponType;
			MulticastSetWeaponSkeletalMesh(false, WeaponObject->WeaponSkeletalMesh, WeaponObject->OffHandAttatchTransform);

			// TODO: remake this wih new procedural generated GE
			// Apply Equip weapon GE for attack interval
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(EquipWeaponOffameplayEffect, 1, EffectContext);
			
			// set by caller with weapon interval to correct attribute, leave other as is 
			if (NewHandle.IsValid())
			{
				FGameplayEffectSpec* GESpec = NewHandle.Data.Get();
				FGameplayTag LeftHandIntervalTag = FGameplayTag::RequestGameplayTag(FName("Data.Attribute.OffHandAttackInterval"));
				GESpec->SetSetByCallerMagnitude(LeftHandIntervalTag, WeaponObject->AttackInterval);
			
				FGameplayTag LeftHandWeaponPowerTag = FGameplayTag::RequestGameplayTag(FName("Data.Attribute.OffHandWeaponPower"));
				GESpec->SetSetByCallerMagnitude(LeftHandWeaponPowerTag, WeaponObject->WeaponPower);
			
				WeaponOffActiveGE = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
			}
		}
	}
}

void AProject4Character::ResetRightHandWeaponInfo()
{
	
	MulticastSetWeaponSkeletalMesh(true, nullptr, FTransform());
	AbilitySystemComponent->RemoveActiveGameplayEffect(WeaponMainActiveGE, 1);
	MeshRH->ResetRelativeTransform();
	//AbilitySystemComponent->RemoveActiveEffectsWithTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Equippment.Weapon.MainHand")));
}

void AProject4Character::ResetLeftHandWeaponInfo()
{
	MulticastSetWeaponSkeletalMesh(false, nullptr, FTransform());
	MeshLH->ResetRelativeTransform();
	AbilitySystemComponent->RemoveActiveGameplayEffect(WeaponOffActiveGE, 1);
	//AbilitySystemComponent->RemoveActiveEffectsWithTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Equippment.Weapon.OffHand")));
}

void AProject4Character::MulticastSetWeaponSkeletalMesh_Implementation(bool IsRightHand, USkeletalMesh* SkeletalMesh, const FTransform& Transform)
{
	UpdateAnimationOverlay(); // TODO: move this to own multicast or on_rep_notify on weapon type changes

	if (SkeletalMesh)
	{
		if (IsRightHand)
		{
			MeshRH->SetVisibility(true, true);
			MeshRH->SetSkeletalMesh(SkeletalMesh);
			MeshRH->SetRelativeTransform(Transform);
		}
		else
		{
			MeshLH->SetVisibility(true, true);
			MeshLH->SetSkeletalMesh(SkeletalMesh);
			MeshLH->SetRelativeTransform(Transform);
		}
	}
	else
	{
		if (IsRightHand)
		{
			MeshRH->SetVisibility(false, true);
		}
		else
		{
			MeshLH->SetVisibility(false, true);
		}
	}

}

void AProject4Character::PlayStunnedAnimationMontage()
{
	if (StunnedMontage && !HasAuthority())
	{
		PlayAnimMontage(StunnedMontage);
	}
}


void AProject4Character::BeginPlay()
{
	Super::BeginPlay();


	if(AbilitySystemComponent.IsValid())
		AbilitySystemComponent->SetOwnerActor(this);

	//AddAllStartupEffects();
	// FIXME: set all visibility to false, let render dist sphere collision set vis to true when needed
	// Only do this to actors that player isnt locally controlling
	//APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//if (PC && PC->IsLocalPlayerController() && PC != GetController<APlayerController>())
	//{
	//	RootComponent->SetVisibility(false, true);
	//}
}




/***************************/
/* Gameplay Ability system */
/***************************/



void AProject4Character::GiveEssentialAbilities()
{
	if (EssentialAbilities && HasAuthority()) {
		EssentialAbilities->GiveAbilities(GetAbilitySystemComponent());
	}
}

void AProject4Character::AddAllStartupEffects()
{
	if (HasAuthority() && AbilitySystemComponent.IsValid() && !StartupEffectsApplied)	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
		{
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
			}
		}

		StartupEffectsApplied = true;
	}	
}

void AProject4Character::InitializeAttributeSet()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitStats(UP4BaseAttributeSet::StaticClass(), AttrDataTable);
	}

}

void AProject4Character::ApplyRespawnGameplayEffects()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	for (TSubclassOf<UGameplayEffect> GameplayEffect : OnRespawnGEs)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
}

void AProject4Character::ApplyDeathGameplayEffects()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	for (TSubclassOf<UGameplayEffect> GameplayEffect : OnDeathGEs)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
}

void AProject4Character::RemoveDeathGameplayEffects()
{
	for (auto e : OnDeathGEHandles)
		AbilitySystemComponent->RemoveActiveGameplayEffect(e);
}


// ovveride replciation with replication variables
void AProject4Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(AProject4Character, SelectedTarget);
	DOREPLIFETIME(AProject4Character, WeaponStance);
	DOREPLIFETIME(AProject4Character, MainHandWeaponType);
	DOREPLIFETIME(AProject4Character, OffHandWeaponType);
}






