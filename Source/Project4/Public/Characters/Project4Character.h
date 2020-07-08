// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Project4Character.generated.h"


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
	UPROPERTY(VisibleAnywhere, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Attributes)
		TWeakObjectPtr<class UPlayerAttributeSet> AttributeSet;

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshLH;

	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshRH;

public:
	AProject4Character();

	/***************************/
	/* Gameplay Ability system */  
	/***************************/

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = Ability, meta = (DefaultToSelf = Target))
		class UPlayerAttributeSet* GetAttributeSet() const;

	// Simple check if health
	UFUNCTION(BlueprintCallable)
		virtual bool IsAlive() const;


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

	// Called on actorspawn ONLY servers need to call this and startupeffects
	virtual	void GiveEssentialAbilities();

	// Called on actorSpawn, GE's shouldn't be canceled unless we make skills to stop regen
	virtual void AddAllStartupEffects();

public:
	/* Virtual Overrides */

	/* Event Tick Override */
	virtual void Tick(float DeltaTime) override;

	/* override BeginPlay */
	virtual void BeginPlay() override;
};

