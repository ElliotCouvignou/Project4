// Copyright Epic Games, Inc. All Rights Reserved.
// define a print message function to print to screen

#include "Project4Character.h"
#include "Project4Controller.h"
#include "UI/GameplayHUD.h"
#include "Project4.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Project4PlayerState.h"

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

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	CameraZoomGranularity = 50.f;
	CameraZoomMin = 150.f;
	CameraZoomMax = 1500.f;
	CameraSensitivity = 5.f;

	doInputRotateCamera = false;
	doRotatePlayerAndCamera = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 100.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	/* GAS INITS */
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

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
	return AbilitySystemComponent;
}

/***************************/
/*    Input Handlers       */
/***************************/

void AProject4Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AProject4Character::CameraZoom);
	
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AProject4Character::HandleLeftClickPressed);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AProject4Character::HandleLeftClickReleased);
	
	// -------------------------------------------------------------------------------------------
	//			GameplayAbility System Bindings
	// -------------------------------------------------------------------------------------------
	BindASCInput();
}

void AProject4Character::HandleLeftClickPressed()
{
	// TODO: fill this with handler to decide on camera rotation or target selection
	SelectTargetFromCursor();
	
}

void AProject4Character::HandleLeftClickReleased()
{
	// TODO: fill this with handler to decide on camera rotation or target selection
}



void AProject4Character::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent) {

		// Bind Abilities (Remove once we get skill trees or unlock to get skills)
		FGameplayAbilityActorInfo* actorInfo = new FGameplayAbilityActorInfo();
		actorInfo->InitFromActor(this, this, AbilitySystemComponent);
		AbilitySystemComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(actorInfo);

		AbilitySystemComponent->InitAbilityActorInfo(this, this);


		// Init playerAttributes with .csv
		const UAttributeSet* Attrs = AbilitySystemComponent->InitStats(UPlayerAttributeSet::StaticClass(), AttrDataTable);
	}

	if (IsLocallyControlled()) {
		AProject4Controller* PC = Cast<AProject4Controller>(GetController());
		if (PC) {
			HUD = Cast<AGameplayHUD>(PC->GetHUD());
		}
	}
}

// Called every frame
void AProject4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ovveride replciation with replication variables
void AProject4Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(AProject4Character, SelectedTarget);
	DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(AProject4Character, BoundAbilities);
	DOREPLIFETIME(AProject4Character, AbilitySpecHandles);


}

/* Server Event: Apply Damage to Character (Inputs raw damage)*/
/* Take Damage Override Function, Only executed from server */
float AProject4Character::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//AttributeSet->Health.SetCurrentValue(AttributeSet->Health.GetCurrentValue() - Damage);
	return Damage;
}

void AProject4Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// essential for networking, make sure ability stays linked 
	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		AddAllStartupEffects();
		GiveEssentialAbilities(this);

		//AProject4PlayerState* PS = Cast<AProject4PlayerState>(GetPlayerState());
		//if (PS)
		//{
		//	PS->SetAbilitySystemComponent(AbilitySystemComponent);
		//}

	}

}

void AProject4Character::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Changed PlayerController, update AbilitySystemCompnent
	if (AbilitySystemComponent) {
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}


/***************************/
/* Gameplay Ability system */
/***************************/


void AProject4Character::AddAllCharacterAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent) {
		return;
	}

	int idx = 0;
	for (TSubclassOf<UP4GameplayAbility>& StartupAbility : BoundAbilities) {
		if (BoundAbilities[idx]) {
			AbilitySpecHandles[idx] = AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(BoundAbilities[idx], 1, ABILITY_INPUT_OFFSET + idx, this));
		}
		idx += 1;
	}
}


void AProject4Character::AddAllStartupEffects()
{
	if (!HasAuthority()|| !AbilitySystemComponent)
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
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void AProject4Character::GivePlayerAbilityToBlock_Implementation(AProject4Character* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability)
{
	if (TargetActor->AbilitySystemComponent) {
		// check current block, if ability is bound then clear
		if (TargetActor->AbilitySpecHandles.IsValidIndex(BlockIndex) && (TargetActor->AbilitySpecHandles[BlockIndex].IsValid())){
			TargetActor->AbilitySystemComponent->ClearAbility(AbilitySpecHandles[BlockIndex]);
		}

		// Bind ability if it isnt nullptr
		if (Ability)
		{
			TargetActor->BoundAbilities.Insert(Ability, BlockIndex);
			FGameplayAbilitySpecHandle NewAbilityHandle = AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(BoundAbilities[BlockIndex], 1, ABILITY_INPUT_OFFSET + BlockIndex, this));
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

void AProject4Character::InitBoundAbilityArrays_Implementation(AProject4Character* TargetActor)
{
	TargetActor->BoundAbilities.SetNum(ABILITY_BLOCK_AMOUNT);
	TargetActor->BoundAbilities.Reset();

	TargetActor->AbilitySpecHandles.SetNum(ABILITY_BLOCK_AMOUNT);
	TargetActor->AbilitySpecHandles.Reset();
}

void AProject4Character::GiveEssentialAbilities_Implementation(AProject4Character* TargetActor)
{
	if (EssentialAbilities) {
		EssentialAbilities->GiveAbilities(GetAbilitySystemComponent());
	}
}


void AProject4Character::BindASCInput()
{
	if (!bASCInputBound && AbilitySystemComponent && IsValid(InputComponent)) {
		FGameplayAbilityInputBinds AbilityBinds("ConfirmAbility", "CancelAbility", "EP4AbilityInputID", 
			static_cast<int32>(EP4AbilityInputID::Confirm), static_cast<int32>(EP4AbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, AbilityBinds);
		
		bASCInputBound = true;
	}
}


/***************************/
/*    Targeting system     */
/***************************/


void AProject4Character::SelectTargetFromCursor()
{
	
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	
	FHitResult HitResult;
	PController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	
	// only allow this class and children of this class to be selectable
	AActor* HitActor = HitResult.GetActor();
	if (Cast<AProject4Character>(HitActor)) {
		ServerSetSelectedTarget(this, HitResult.GetActor());
		SelectedTarget = HitResult.GetActor();

		// Update Target Selected Widget with new information
		if (IsLocallyControlled()) {
			HUD->SetNewTargetData(SelectedTarget);
		}
	}
	else {
		// Unselect target, set target widget off
		if (IsLocallyControlled())
			HUD->SetNewTargetData(nullptr);

		SelectedTarget = nullptr; 
	}	
}

void AProject4Character::SelectNextNearestTarget()
{
	// TODO Implement Tab-style targeting, make sure to grab next nearest
}


/*   Replication Area   */

void AProject4Character::ServerSetSelectedTarget_Implementation(AProject4Character* TargetedActor, AActor* NewSelectedTarget)
{
	TargetedActor->SelectedTarget = NewSelectedTarget;
}


/***************************/
/*      Camera system      */
/***************************/



void AProject4Character::CameraZoom(float Value)
{
	CameraBoom->TargetArmLength = FMath::Clamp(Value * CameraZoomGranularity + CameraBoom->TargetArmLength, CameraZoomMin, CameraZoomMax);
}

//// BELOW WAS AUTO GENERATED ////

void AProject4Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}





void AProject4Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FVector Direction = AActor::GetActorForwardVector();
		AddMovementInput(Direction, Value);
	}
}

void AProject4Character::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FVector Direction = AActor::GetActorRightVector();

		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}









/* UNUSED ATM FUNCTIONS */





// this and next two handle left click to rotate camera and other inclusive functions that need call
void AProject4Character::AddInputToCameraRotation()
{
	float deltaX, deltaY;

	GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(deltaX, deltaY);

	FRotator Rotation = CameraBoom->GetRelativeRotation();
	//Rotation.Pitch = FMath::Clamp((Rotation.Pitch + deltaY * CameraSensitivity), -75.f, 50.f);
	Rotation.Yaw += deltaX * CameraSensitivity;

	CameraBoom->SetRelativeRotation(Rotation);
}

void AProject4Character::StartMovePlayerCamera() {
	doInputRotateCamera = true;
}


void AProject4Character::StopMovePlayerCamera() {
	doInputRotateCamera = false;
}




// This and next to handle right click funcitonality of setting the player's rotaiton to camera rotation
void AProject4Character::SetPlayerRotationToCamera()
{
	// Rotation actually stored in the camera boom
	if (GetController() != NULL /*&& ISCLIENT*/) {
		FRotator Rot = GetController()->GetControlRotation();
		Rot.Yaw = CameraBoom->GetTargetRotation().Yaw;
		//print(FString::SanitizeFloat(Rot.Yaw, 4));
		GetController()->SetControlRotation(Rot);
	}
}

void AProject4Character::StartPlayerRotationToCamera()
{
	doRotatePlayerAndCamera = true;
}

void AProject4Character::StopPlayerRotationToCamera()
{
	doRotatePlayerAndCamera = false;
}