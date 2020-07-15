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

#include "GameplayAbilitySpec.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystem/PlayerAttributeSet.h"
#include "AbilitySystem/P4GameplayAbilitySet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"




#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


//////////////////////////////////////////////////////////////////////////
// AProject4Character

AProject4Character::AProject4Character(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

	MeshRH = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshRH"));
	MeshRH->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("hand_r_Socket")));

	UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIFloatingStatusBarComponent"));
	UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
	UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0, 0, 120));
	UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500, 500));

	DeadTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.Dead"));
	//Respawn = FGameplayTag::RequestGameplayTag(FName(""));
}

UAbilitySystemComponent* AProject4Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}


UPlayerAttributeSet* AProject4Character::GetAttributeSet() const
{
	return AttributeSet.Get();
}



bool AProject4Character::IsAlive() const
{
	if (AttributeSet.Get()) {
		if (AttributeSet->GetHealth() > 0.f)
		{
			return true;
		}
	}
	return false;
}

void AProject4Character::Die()
{
	// fix issue crash when 2+ clients
	//RemoveAllAbilitites();

	// Stop player and remove collisions, freeze in air
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCharacterMovement()->GravityScale = 0;
	//GetCharacterMovement()->Velocity = FVector(0);

	// broadcast delegate I ded
	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		// stop all abilitites regardless of tags (not GE's)
		AbilitySystemComponent->CancelAllAbilities();

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	// play death montage if set, else play ALS ragdoll with manual delay
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		// calls ALS to do ragdoll
		ActivateRagdoll();
		GetWorld()->GetTimerManager().SetTimer(RadgollDeathHandle, this, &AProject4Character::FinishDying, RadgollDeathDelay, false);
	}
}

void AProject4Character::FinishDying()
{
	//Destroy();
}

void AProject4Character::Respawn_Implementation()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		int32 count = AbilitySystemComponent->GetTagCount(DeadTag);
	}
	if (!DeathMontage)
	{
		// HACK: kindof assuming we are in ragdoll to undo it
		ActivateRagdoll();
	}
}

/***************************/
/*           UI            */
/***************************/

void AProject4Character::InitFloatingStatusBarWidget()
{
	if (AbilitySystemComponent.IsValid())
	{
		// Setup FloatingStatusBar UI for Locally Owned Players only but exclude your own FSB
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->IsLocalPlayerController() && !IsLocallyControlled())
		{
			if (UIFloatingStatusBarClass)
			{
				UIFloatingStatusBar = CreateWidget<UFloatingStatusBarWidget>(PC, UIFloatingStatusBarClass);
				if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
				{
					UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);
					UIFloatingStatusBarComponent->SetDrawSize(UIFloatingStatusBar->GetDesiredSize());

					// Setup the floating status bar
					UIFloatingStatusBar->SetHealthPercentage(AttributeSet->GetHealth() / AttributeSet->GetHealthMax());

					// TODO: set real character name in floating bar
					//UIFloatingStatusBar->SetCharacterName(CharacterName);
				}
			}
		}
	}
}

UFloatingStatusBarWidget* AProject4Character::GetFloatingStatusBarWidget()
{
	return UIFloatingStatusBar;
}


void AProject4Character::BeginPlay()
{
	Super::BeginPlay();

	
}




/***************************/
/* Gameplay Ability system */
/***************************/

void AProject4Character::RemoveAllAbilitites()
{
	if (!HasAuthority() || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	TArray<FGameplayAbilitySpec> AllAbilities = AbilitySystemComponent->GetActivatableAbilities();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	// remove essential abilities (Player abilities removed in player class virtual)
	for (const FP4GameplayAbilityBindInfo& Ability : EssentialAbilities->Abilities)
	{
		FGameplayAbilitySpec* spec = ASC->FindAbilitySpecFromClass(Ability.AbilityClass);
		if (spec)
		{
			AbilitiesToRemove.Add(spec->Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	// prevent player from inputting abilities

}

void AProject4Character::GiveEssentialAbilities()
{
	if (EssentialAbilities && HasAuthority()) {
		EssentialAbilities->GiveAbilities(GetAbilitySystemComponent());
	}
}

void AProject4Character::AddAllStartupEffects()
{
	if (!HasAuthority()|| !AbilitySystemComponent.IsValid())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}
}

void AProject4Character::InitializeAttributeSet()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitStats(UPlayerAttributeSet::StaticClass(), AttrDataTable);
	}

}


// ovveride replciation with replication variables
void AProject4Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(AProject4Character, SelectedTarget);
}






