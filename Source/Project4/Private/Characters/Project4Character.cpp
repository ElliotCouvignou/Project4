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
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "UI/GameplayHudWidget.h"

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

AProject4Character::AProject4Character()
{
	// Netoworking characteristics for character
	bReplicates = true;
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
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



void AProject4Character::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent.Get()) {

		// Bind Abilities (Remove once we get skill trees or unlock to get skills)
		FGameplayAbilityActorInfo* actorInfo = new FGameplayAbilityActorInfo();
		actorInfo->InitFromActor(this, this, AbilitySystemComponent.Get());
		AbilitySystemComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(actorInfo);

		AbilitySystemComponent->InitAbilityActorInfo(this, this);


		// Init playerAttributes with .csv
		const UAttributeSet* Attrs = AbilitySystemComponent->InitStats(UPlayerAttributeSet::StaticClass(), AttrDataTable);		
	}

}

// Called every frame
void AProject4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



/***************************/
/* Gameplay Ability system */
/***************************/

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



void AProject4Character::GiveEssentialAbilities()
{
	if (EssentialAbilities) {
		EssentialAbilities->GiveAbilities(GetAbilitySystemComponent());
	}
}


// ovveride replciation with replication variables
void AProject4Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProject4Character, AttributeSet);
}






