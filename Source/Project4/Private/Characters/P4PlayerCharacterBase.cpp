// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/P4PlayerCharacterBase.h"
#include "Characters/Project4Character.h"
#include "Project4.h"
#include "Project4Controller.h"
#include "Project4PlayerState.h"
#include "Project4GameMode.h"
#include "Net/UnrealNetwork.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"

#include "Interactables/P4InventoryBagComponent.h"
#include "Characters/SkillTreeComponent.h"

#include "UI/GameplayHudWidget.h"


#define CAMERA_ZOOM_MIN 100.f
#define CAMERA_ZOOM_MAX 750.f
#define CAMERA_ZOOM_GRANULARITY 50.f



AP4PlayerCharacterBase::AP4PlayerCharacterBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 100.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Player Inventory Component
	InventoryBagComponent = CreateDefaultSubobject<UP4InventoryBagComponent>(TEXT("InventoryBagComponent"));
	InventoryBagComponent->SetIsReplicated(true);

	SkillTreeComponent = CreateDefaultSubobject<USkillTreeComponent>(TEXT("SkillTreeComponent"));
	SkillTreeComponent->SetIsReplicated(true);
	

}

void AP4PlayerCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AP4PlayerCharacterBase::CameraZoom);


	// Bind ASC Input
	BindASCInput();
}

void AP4PlayerCharacterBase::BindASCInput()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	
	if (!bASCInputBound && ASC && IsValid(InputComponent)) {
		FGameplayAbilityInputBinds AbilityBinds("ConfirmAbility", "CancelAbility", "EP4AbilityInputID",
			static_cast<int32>(EP4AbilityInputID::Confirm), static_cast<int32>(EP4AbilityInputID::Cancel));

		ASC->BindAbilityActivationToInputComponent(InputComponent, AbilityBinds);

		bASCInputBound = true;
	}
}


void AP4PlayerCharacterBase::FinishDying()
{
	if (HasAuthority())
	{
		AProject4GameMode* GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->PlayerDeath(GetController());
		}
	}
	// Call super at end since we dont want actor destroyed 
	// for a bit
	Super::FinishDying();
}


void AP4PlayerCharacterBase::GainExperience(int XpGained)
{
	if (!HasAuthority())
	{
		return;
	}
	UPlayerAttributeSet* AS = GetAttributeSet();

	float NewXp = AS->GetExperience() + XpGained;
	if (NewXp >= AS->GetExperienceMax())
	{
		// Player Level Up
		NewXp -= AS->GetExperienceMax();
		float NewLevel = AS->GetLevel() + 1.f;
		AS->SetLevel(NewLevel);

		// TODO: finalize max xp formula
		float NewMaxXP = FMath::Square(AS->GetLevel()) / 4.f + 10.f*AS->GetLevel() + 150.f;
		AS->SetExperienceMax(NewMaxXP);
	}
	AS->SetExperience(NewXp);
}

void AP4PlayerCharacterBase::BindAbilityToHotbarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability)
{
	if (IsLocallyControlled())
	{
		AProject4Controller* PC = Cast<AProject4Controller>(GetController());
		if (PC)
		{
			PC->SetupUIAbilityToHotBarBlock(BlockIndex, Ability);
		}
		BindAbilityToHotbarInput(this, BlockIndex, Ability);
	}
}

void AP4PlayerCharacterBase::BindAbilityToHotbarInput_Implementation(AP4PlayerCharacterBase* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability)
{
	UAbilitySystemComponent* ASC = TargetActor->GetAbilitySystemComponent();
	if (ASC) {
		// check current block, if ability is bound then clear
		if (TargetActor->AbilitySpecHandles.IsValidIndex(BlockIndex) && (TargetActor->AbilitySpecHandles[BlockIndex].IsValid())) {
			ASC->ClearAbility(AbilitySpecHandles[BlockIndex]);
		}

		// Bind ability if it isnt nullptr
		if (Ability)
		{
			TargetActor->BoundAbilities.Insert(Ability, BlockIndex);
			FGameplayAbilitySpecHandle NewAbilityHandle = ASC->GiveAbility(
				FGameplayAbilitySpec(Ability, 1, ABILITY_INPUT_OFFSET + BlockIndex, TargetActor));
			TargetActor->AbilitySpecHandles.Insert(NewAbilityHandle, BlockIndex);
		}
		else
		{
			// Set block to nothing
			TargetActor->BoundAbilities.Insert(nullptr, BlockIndex);
			TargetActor->AbilitySpecHandles.Insert(FGameplayAbilitySpecHandle(), BlockIndex);
		}
	}
}

void AP4PlayerCharacterBase::InitBoundAbilityArrays_Implementation(AP4PlayerCharacterBase* TargetActor)
{
	TargetActor->BoundAbilities.Reset();
	TargetActor->BoundAbilities.SetNum(ABILITY_BLOCK_AMOUNT);
	
	TargetActor->AbilitySpecHandles.Reset();
	TargetActor->AbilitySpecHandles.SetNum(ABILITY_BLOCK_AMOUNT);
}

void AP4PlayerCharacterBase::AddAllCharacterAbilities()
{
	if (!HasAuthority() || !GetAbilitySystemComponent()) {
		return;
	}

	int idx = 0;
	for (TSubclassOf<UP4GameplayAbility>& StartupAbility : BoundAbilities) {
		if (StartupAbility) {
			AbilitySpecHandles[idx] = GetAbilitySystemComponent()->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1, ABILITY_INPUT_OFFSET + idx, this));
		}
		idx += 1;
	}
}

/***************************/
/*    Targeting system     */
/***************************/


void AP4PlayerCharacterBase::SelectTargetFromCursor()
{

	AProject4Controller* PController = Cast<AProject4Controller>(GetController());

	FHitResult HitResult;
	PController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);

	// only allow this class and children of this class to be selectable
	AActor* HitActor = HitResult.GetActor();
	if (Cast<AProject4Character>(HitActor)) {
		ServerSetSelectedTarget(this, HitResult.GetActor());
		SelectedTarget = HitResult.GetActor();

		// Update Target Selected Widget with new information
		if (IsLocallyControlled()) {
			PController->GetMainHUDWidget()->SetNewTargetData(SelectedTarget);
		}
	}
	else {
		// Unselect target, set target widget off
		if (IsLocallyControlled())
			PController->GetMainHUDWidget()->SetNewTargetData(nullptr);

		SelectedTarget = nullptr;
	}
}



/*   Replication Area   */

void AP4PlayerCharacterBase::ServerSetSelectedTarget_Implementation(AP4PlayerCharacterBase* TargetedActor, AActor* NewSelectedTarget)
{
	TargetedActor->SelectedTarget = NewSelectedTarget;
}


/***************************/
/*      Camera system      */
/***************************/
void AP4PlayerCharacterBase::CameraZoom(float Value)
{
	CameraBoom->TargetArmLength = FMath::Clamp(Value * CAMERA_ZOOM_GRANULARITY + CameraBoom->TargetArmLength, CAMERA_ZOOM_MIN, CAMERA_ZOOM_MAX);
}

// ovveride replciation with replication variables
void AP4PlayerCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(AP4PlayerCharacterBase, BoundAbilities);
	DOREPLIFETIME(AP4PlayerCharacterBase, AbilitySpecHandles);
}



/* Virtual Overrides */
void AP4PlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called upon new player controller related to this character 
// (owning client)
void AP4PlayerCharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Changed PlayerController, update AbilitySystemCompnent
	if (AbilitySystemComponent.IsValid()) {
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

// Server only, Clients process below function
// this has server only functions being called
void AP4PlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AProject4PlayerState* PS = GetPlayerState<AProject4PlayerState>();

	if (PS) {
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set AttributeSet
		AttributeSet = PS->GetAttributeSet();
		
		// Tell PS to bind delegates before init
		PS->BindAbilityDelegates();


		// Init playerAttributes with .csv
		InitializeAttributeSet();

		BindASCInput();

		// do startuf effects and essential ability setup
		AddAllStartupEffects();

		// For edge cases where the PlayerState is repped before the Hero is possessed.
		// Maybe dont use this for servers? some might still need a ref for some reason
		AProject4Controller* PC = Cast<AProject4Controller>(GetController());
		if (PC)
		{
			PC->CreateMainHUDWidget();
		}

		GiveEssentialAbilities();

		InitFloatingStatusBarWidget();

	}
}

// Called upon new playerstate related to this character 
// (All clients)
void AP4PlayerCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AProject4PlayerState* PS = GetPlayerState<AProject4PlayerState>();

	if (PS) {
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UAbilitySystemComponent>(PS->GetAbilitySystemComponent());


		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Set AttributeSet
		AttributeSet = PS->GetAttributeSet();

		// Tell PS to bind delegates before init
		PS->BindAbilityDelegates();


		// Init playerAttributes with .csv
		InitializeAttributeSet();


		// For edge cases where the PlayerState is repped before the Hero is possessed.
		// Maybe dont use this for servers? some might still need a ref for some reason
		AProject4Controller* PC = Cast<AProject4Controller>(GetController());
		if (PC)
		{
			PC->CreateMainHUDWidget();
		}

		BindASCInput();

		InitFloatingStatusBarWidget();
	}
}