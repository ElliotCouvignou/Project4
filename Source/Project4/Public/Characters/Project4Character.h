// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Project4Character.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AProject4Character*, Character);

/**
* The base Character class for the game. Everything with an AbilitySystemComponent in this game will inherit from this class.
* This class should not be instantiated and instead subclassed.
*/
UCLASS(config = Game)
class AProject4Character : public ACharacter, public IAbilitySystemInterface
{

	GENERATED_BODY()

protected:

	/***************************/
	/*       Components        */
	/***************************/

	// Character ASC, this is shared  players and mob classes
	// No UPROPERTY for these, makes their UObject references go stale when out of scope
	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UPlayerAttributeSet> AttributeSet;

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshLH;

	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshRH;

public:
	AProject4Character(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
		FCharacterDiedDelegate OnCharacterDied;

	/***************************/
	/* Gameplay Ability system */  
	/***************************/

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = Ability, meta = (DefaultToSelf = Target))
		class UPlayerAttributeSet* GetAttributeSet() const;

	/***************************/
	/*          Death          */
	/***************************/

	// Simple check if health
	UFUNCTION(BlueprintCallable)
		virtual bool IsAlive() const;

	virtual void Die();

	// blueprintcallable for anim notify
	UFUNCTION(BlueprintCallable, Category = Death)
		virtual	void FinishDying();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Death)
		void ActivateRagdoll();

	// used by gamemode to undo ragdoll IF no death montage
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void UndoRagdoll();
	void UndoRagdoll_Implementation();
	bool UndoRagdoll_Validate() { return true; }

	/***************************/
	/*     Targeting system    */
	/***************************/
	
	// Pointer to selected target, Mobs will use this too for their abilitites
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Abilities)
		AActor* SelectedTarget;

protected:

	/***************************/
	/* Gameplay Ability system */
	/***************************/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attributes)
		UDataTable* AttrDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
		class UP4GameplayAbilitySet* EssentialAbilities;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// unbinds all active abilities, Occurs on death
	virtual void RemoveAllAbilitites();

	// Called on actorspawn ONLY servers need to call this and startupeffects
	virtual	void GiveEssentialAbilities();

	// Called on actorSpawn, GE's shouldn't be canceled unless we make skills to stop regen
	virtual void AddAllStartupEffects();

	// Init playerAttributes with .csv
	void InitializeAttributeSet();

	/***************************/
	/*          Death          */
	/***************************/

	// delay from ragdoll in die() to FinishDying()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float RadgollDeathDelay = 5.f;

	FTimerHandle RadgollDeathHandle;

	// optinal death animation montage (doesnt exist -> ALS ragdoll)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UAnimMontage* DeathMontage;

	FGameplayTag DeadTag;
	FGameplayTag RespawnTag;

public:
	/* Virtual Overrides */

	/* override BeginPlay */
	virtual void BeginPlay() override;
};

