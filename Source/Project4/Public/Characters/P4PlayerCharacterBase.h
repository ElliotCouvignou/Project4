// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Project4Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "P4PlayerCharacterBase.generated.h"

/**
 *  Base classe for playable characters, this should be instantiated and subclassed if needed
 */
UCLASS()
class PROJECT4_API AP4PlayerCharacterBase : public AProject4Character
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


public:
	AP4PlayerCharacterBase(const class FObjectInitializer& ObjectInitializer);

	/***************************/
	/* Gameplay Ability system */
	/***************************/

	// Called on new hotbar ability assignment, can change input bindings and replace
	// Abilities with new bindings. Does remove old abilities in spot if exists
	// Adding abilities requires server control, so call server and it will replicate for us
	// However we need client work so clients need to do this function and call servers
	UFUNCTION(BLueprintCallable)
		void BindAbilityToHotbarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);
	
	// helper for above, calls on ASC from server to do input bindings based on hotbar bindings
	UFUNCTION(Server, Reliable, WithValidation, Category = Abilities)
		void BindAbilityToHotbarInput(AP4PlayerCharacterBase* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);
	virtual bool BindAbilityToHotbarInput_Validate(AP4PlayerCharacterBase* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability) { return true; }
	virtual void BindAbilityToHotbarInput_Implementation(AP4PlayerCharacterBase* TargetActor, int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);

	
	// sets array sizes to # ability blocks and then resets array values
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = Abilities)
		void InitBoundAbilityArrays(AP4PlayerCharacterBase* TargetActor);
	virtual bool InitBoundAbilityArrays_Validate(AP4PlayerCharacterBase* TargetActor) { return true; }
	virtual void InitBoundAbilityArrays_Implementation(AP4PlayerCharacterBase* TargetActor);

	/***************************/
	/*      Camera system      */
	/***************************/

	void CameraZoom(float Value);

	/***************************/
	/*          Death          */
	/***************************/

	virtual void FinishDying() override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/***************************/
	/* Gameplay Ability system */
	/***************************/

	// 'sorted' Array to hold all bound abilities, Index 0 = UseAbility1, 9 = UseAbility0. 
	UPROPERTY(BlueprintReadOnly, Replicated, EditAnywhere, Category = Abilities)
		TArray<TSubclassOf<class UP4GameplayAbility>> BoundAbilities;

	// 'sorted' in same fashion above, this is essential
	UPROPERTY(BlueprintReadOnly, Replicated, EditAnywhere, Category = Abilities)
		TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// grants and binds binds hotbar abilitites 
	// TODO expose this to grant learned (but not bound) abilities
	virtual void AddAllCharacterAbilities();

	void BindASCInput();
	bool bASCInputBound = false;

	/***************************/
	/*    Targeting system     */
	/***************************/

	UFUNCTION()
		void SelectTargetFromCursor();  // For left-click selection
	UFUNCTION()
		void SelectNextNearestTarget(); // For tab-targeting

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetSelectedTarget(AP4PlayerCharacterBase* TargetedActor, AActor* NewSelectedTarget);    // replciated Selected Target
	virtual bool ServerSetSelectedTarget_Validate(AP4PlayerCharacterBase* TargetedActor, AActor* NewSelectedTarget) { return true; }
	virtual void ServerSetSelectedTarget_Implementation(AP4PlayerCharacterBase* TargetedActor, AActor* NewSelectedTarget);

	/***************************/
	/*    Input Handlers       */
	/***************************/

	UFUNCTION()
		void HandleLeftClickPressed();
	UFUNCTION()
		void HandleLeftClickReleased();


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate = 45.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraSensitivity = 3.f;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


public:
	/* Virtual Overrides */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void PostInitializeComponents() override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	/* Override Controller Posession*/
	virtual void OnRep_Controller() override;

	// called by clients
	virtual void OnRep_PlayerState() override;

};
