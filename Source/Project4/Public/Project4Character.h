// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "PlayerAttributeSet.h"
#include "PlayerAbilitySet.h"
#include "P4GameplayAbility.h"
#include "Project4Character.generated.h"



UCLASS(config = Game)
class AProject4Character : public ACharacter, public IAbilitySystemInterface
{

	GENERATED_BODY()

	/***************************/
	/*       Components        */
	/***************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystem;

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

	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; };

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = Attributes)
		UPlayerAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attributes)
		UDataTable* AttrDataTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// 'sorted' Array to hold all bound abilities, Index 0 = UseAbility1, 9 = UseAbility0. 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<TSubclassOf<class UP4GameplayAbility>> BoundAbilities;

	// 'sorted' in same fashion above, this is essential
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Called on new hotbar ability assignment, can change input bindings and replace
	// Abilities with new bindings. Does remove old abilities in spot if exists
	// Adding abilities requires server control, so call server and it will replicate for us
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = Abilities)
		void GivePlayerAbility(AProject4Character* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);
	virtual bool GivePlayerAbility_Validate(AProject4Character* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);
	virtual void GivePlayerAbility_Implementation(AProject4Character* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);


	/***************************/
	/*    Targeting system     */
	/***************************/

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Abilities)
		AActor* SelectedTarget;

	UPROPERTY()
		class AGameplayHUD* HUD;

	/***************************/
	/*      Camera system      */
	/***************************/

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

	/***************************/
	/*    Input Handlers       */
	/***************************/

	UFUNCTION()
		void HandleLeftClickPressed();
	UFUNCTION()
		void HandleLeftClickReleased();

	/***************************/
	/* Gameplay Ability system */
	/***************************/

	bool bASCInputBound = false;

	// grants abilities (maybe move to hotbar system or call it)
	virtual void AddAllCharacterAbilities();

	// Called on actorSpawn, GE's shouldn't be canceled unless we make skills to stop regen
	virtual void AddAllStartupEffects();


	void BindASCInput();

	/***************************/
	/*    Targeting system     */
	/***************************/

	UFUNCTION()
		void SelectTargetFromCursor();  // For left-click selection
	UFUNCTION()
		void SelectNextNearestTarget(); // For tab-targeting
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetSelectedTarget(AProject4Character* TargetedActor, AActor* NewSelectedTarget);    // replciated Selected Target
	virtual bool ServerSetSelectedTarget_Validate(AProject4Character* TargetedActor, AActor* NewSelectedTarget) { return true; }
	virtual void ServerSetSelectedTarget_Implementation(AProject4Character* TargetedActor, AActor* NewSelectedTarget);

	/***************************/
	/*      Camera system      */
	/***************************/

	void CameraZoom(float Value);

	// Binds to on left click pressed
	void AddInputToCameraRotation();
	void StartMovePlayerCamera();
	void StopMovePlayerCamera();

	// binds to right click pressed
	void SetPlayerRotationToCamera();
	void StartPlayerRotationToCamera();
	void StopPlayerRotationToCamera();

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

