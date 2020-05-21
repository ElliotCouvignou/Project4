// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "PlayerAttributeSet.h"
#include "PlayerAbilitySet.h"
#include "Project4Character.generated.h"



UCLASS(config = Game)
class AProject4Character : public ACharacter, public IAbilitySystemInterface
{

	GENERATED_BODY()



		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;



public:
	AProject4Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/* next 3 are variables handling delta, min/max camera zoom values */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraZoomGranularity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraZoomMin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraZoomMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraSensitivity;	


	/***************************/
	/* Gameplay Ability system */
	/***************************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystem;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability; 

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = Attributes)
		UPlayerAttributeSet* AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		UPlayerAbilitySet* AbilitySet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
		UDataTable* AttrDataTable;



	/* unused Camera bools (move inside main_char_BP) */
	UPROPERTY(BlueprintReadWrite)
		bool doInputRotateCamera;
	UPROPERTY(BlueprintReadWrite)
		bool doRotatePlayerAndCamera;


protected:


	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	void CameraZoom(float Value);

	// Binds to on left click pressed
	void AddInputToCameraRotation();
	void StartMovePlayerCamera();
	void StopMovePlayerCamera();

	// binds to right click pressed
	void SetPlayerRotationToCamera();
	void StartPlayerRotationToCamera();
	void StopPlayerRotationToCamera();

	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


public:
	/* Event Tick Override */
	virtual void Tick(float DeltaTime) override;

	/* override take damage to our case (general-cases) */
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/* override BeginPlay */
	virtual void BeginPlay() override;

	/* Override Character Possesion */
	virtual void PossessedBy(AController* NewController) override;
	
	/* Override Controller Posession*/
	virtual void OnRep_Controller() override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

//It's very important that this enum is UENUM, because the code will look for UENUM by the given name and crash if the UENUM can't be found. BlueprintType is there so we can use these in blueprints, too. Just in case. Can be neat to define ability packages.
UENUM(BlueprintType)
namespace EAbilityInput
{
	enum Type
	{
		UseAbility1				UMETA(DisplayName = "TestSpell"),
		UseAbility2				UMETA(DisplayName = "TestSpell2"),
		UseAbility3				UMETA(DisplayName = "Ability3"),
		UseAbility4				UMETA(DisplayName = "Ability4"),
		UseAbility5				UMETA(DisplayName = "Ability5"),
		UseAbility6				UMETA(DisplayName = "Ability6"),
		UseAbility7				UMETA(DisplayName = "Ability7"),
		UseAbility8				UMETA(DisplayName = "Ability8"),
		UseAbility9				UMETA(DisplayName = "Ability9"),
		WeaponAbility			UMETA(DispalyName = "WeaponAutoAttack")
	};
}
