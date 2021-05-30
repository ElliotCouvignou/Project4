// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Project4Character.h"
#include "Project4.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h" 
#include "AbilitySystemInterface.h"
#include "UI/SkillTree/SkillTreeDataAsset.h"
#include "Characters/P4CharacterInfoDataAsset.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory" , meta = (AllowPrivateAccess = "true"))
		class UP4InventoryBagComponent* InventoryBagComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Tree", meta = (AllowPrivateAccess = "true"))
		class USkillTreeComponent* SkillTreeComponent;


public:
	AP4PlayerCharacterBase(const class FObjectInitializer& ObjectInitializer);

	// TODO: DELETE THIS


	/* Function to call animation + vfx when player selects this character on pregamelobby */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Essential")
		void OnCharacterSelected();

	virtual void OnCharacterSelected_Implementation();

	// TODO: remove this its temp-ish
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SetCharacterColor(const FLinearColor& Color);  // For left-click selection

	/* Called during pregame lobby for clients to determine where to place this character (owning player has it centered rest is elsewhere) */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void Mutlicast_SetPreGameLobbyPosition();  // For left-click selection
	void Mutlicast_SetPreGameLobbyPosition_Implementation();  // For left-click selection

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SetPreGameLobbyPosition();  // For left-click selection


	/* Called on by authority */
	UFUNCTION()
		void ServerSetCharacterInfo(const FCharacterInfoStruct& InfoStruct, const EClassAbilityPoolType& PoolType);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void MulticastSetCharacterInfo(const FCharacterInfoStruct& InfoStruct);
	void MulticastSetCharacterInfo_Implementation(const FCharacterInfoStruct& InfoStruct);


	/***************************/
	/* Player Inventory system */
	/***************************/
	UFUNCTION(BlueprintCallable)
		UP4InventoryBagComponent* GetInventoryBagComponent() const { return InventoryBagComponent; }

	/***************************/
	/* Gameplay Ability system */
	/***************************/
	
	/* Levels up player, giving xp, more base stats and some extra points to choose */
	/* Only use this from server */
	UFUNCTION(Category = Abilities)
		void GainExperience(int XpGained);

	// Called on new hotbar ability assignment, can change input bindings and replace
	// Abilities with new bindings. Does remove old abilities in spot if exists
	// Adding abilities requires server control, so call server and it will replicate for us
	// However we need client work so clients need to do this function and call servers
	UFUNCTION(BlueprintCallable)
		void BindAbilityToHotbarBlock(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void BindAbilityToInputID(EP4AbilityInputID InputID, TSubclassOf<class UP4GameplayAbility> Ability);
	void BindAbilityToInputID_Implementation(EP4AbilityInputID InputID, TSubclassOf<class UP4GameplayAbility> Ability);
	bool BindAbilityToInputID_Validate(EP4AbilityInputID InputID, TSubclassOf<class UP4GameplayAbility> Ability) { return true; }


	/***************************/
	/*      Camera system      */
	/***************************/

	void CameraZoom(float Value);

	/***************************/
	/*          Death          */
	/***************************/

	virtual void FinishDying() override;

	UFUNCTION(BlueprintCallable)
		void SetCameraSensitivity(float NewVal) { CameraSensitivity = NewVal; }


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/***************************/
	/* Gameplay Ability system */
	/***************************/

		// Init playerAttributes with .csv
	virtual void InitializeAttributeSet() override;




	/* Array of bound ability classes to hotbar, on input ppress try activate ability through class
		This is only saved on the client */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = GAS)
		TArray<TSubclassOf<class UP4GameplayAbility>> BoundAbilities;

	void BindASCInput();
	bool bASCInputBound = false;

	/***************************/
	/*    Targeting system     */
	/***************************/

	UFUNCTION(BlueprintCallable)
		void SelectTargetFromCursor();  // For left-click selection

	/* client update selected target */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetSelectedTarget(AP4PlayerCharacterBase* TargetedActor, AActor* NewSelectedTarget);    // replciated Selected Target
	virtual bool ServerSetSelectedTarget_Validate(AP4PlayerCharacterBase* TargetedActor, AActor* NewSelectedTarget) { return true; }
	virtual void ServerSetSelectedTarget_Implementation(AP4PlayerCharacterBase* TargetedActor, AActor* NewSelectedTarget);

	/***************************/
	/*    Input Handlers       */
	/***************************/

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate = 45.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraSensitivity = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraBoomMaxHeightOffset = 250.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float CameraBoomMinHeightOffset = 90.f;

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
