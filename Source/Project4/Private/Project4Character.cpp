// Copyright Epic Games, Inc. All Rights Reserved.
// define a print message function to print to screen
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))

#include "Project4Character.h"
#include "Project4.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameplayAbilitySet.h"
#include "AbilitySystemGlobals.h"

//////////////////////////////////////////////////////////////////////////
// AProject4Character

AProject4Character::AProject4Character()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	CameraZoomGranularity = 50.f;
	CameraZoomMin = 150.f;
	CameraZoomMax = 1000.f;
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

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProject4Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AProject4Character::CameraZoom);


	// -------------------------------------------------------------------------------------------
	//			GameplayAbility System Bindings
	// -------------------------------------------------------------------------------------------

	// bind player/client specific input component
	//AbilitySystem->BindToInputComponent(PlayerInputComponent);
	
	//AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "EAbilityInput"));
	FGameplayAbilityInputBinds AbilityBinds("AbilityConfirm", "AbilityCancel", "EAbilityInput");
	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, AbilityBinds);

}

void AProject4Character::BeginPlay()
{
	Super::BeginPlay();

	
	// Bind Abilities (Remove once we get skill trees or unlock to get skills)
	if (AbilitySystem) 
	{
		FGameplayAbilityActorInfo* actorInfo = new FGameplayAbilityActorInfo();
		actorInfo->InitFromActor(this, this, AbilitySystem);
		AbilitySystem->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(actorInfo);

		if (HasAuthority() && Ability)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0, this));
		}
		if (HasAuthority() && AbilitySet)
		{
			AbilitySet->GiveAbilities(AbilitySystem);
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	// Init playerAttributes with .csv
	if (AbilitySystem) {
		const UAttributeSet* Attrs = AbilitySystem->InitStats(UPlayerAttributeSet::StaticClass(), AttrDataTable);
	}
}





// Called every frame
void AProject4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



/* Server Event: Apply Damage to Character (Inputs raw damage)*/
/* Take Damage Override Function, Only executed from server */
float AProject4Character::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//AttributeSet->Health.SetCurrentValue(AttributeSet->Health.GetCurrentValue() - Damage);
	return Damage;
}




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


// ovveride replciation with replication variables
void AProject4Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);


}

void AProject4Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// essential for networking, make sure ability stays linked 
	if (AbilitySystem) {
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

}

void AProject4Character::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Changed PlayerController, update AbilitySystemCompnent
	if (AbilitySystem) {
		AbilitySystem->RefreshAbilityActorInfo();
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