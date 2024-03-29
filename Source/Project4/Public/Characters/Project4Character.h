// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interactables/P4ItemWeaponObject.h"
#include "NiagaraComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Project4Character.generated.h"


/* Represents type of auto attacks we should use (dual weild, 2h, ranged, etc) */
// Item Type (axe,sword,etc) will be determined through gameplayt
UENUM(BlueprintType)
enum class EWeaponStanceType : uint8
{
	// 0 None
	None					UMETA(DisplayName = "None"),  // "None"
	MeleeDualWield			UMETA(DisplayName = "Melee Dual Wield"),
	MeleeMainHandOnly		UMETA(DisplayName = "Melee Main Hand Only"),   // 2h type
	RangedDualWield			UMETA(DisplayName = "Ranged Dual Wield"),
	RangedMainHandOnly		UMETA(DisplayName = "Ranged Main Hand Only")

};


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
	TWeakObjectPtr<class UP4BaseAttributeSet> AttributeSet;

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshLH;

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshRH;



	// Floatingstatusbar is connected to statusbarcomponent
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
		class UFloatingStatusBarWidget* UIFloatingStatusBar;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
		class UWidgetComponent* UIFloatingStatusBarComponent;

	/* Widget for displaying damage/heal texts */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
		class UP4FloatingTextWidget* UIFloatingTextWidget;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
		class UWidgetComponent* FloatingTextWidgetComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "VFX")
		UNiagaraComponent* NiagaraComponent;


public:
	AProject4Character(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
		FCharacterDiedDelegate OnCharacterDied;


	/* Object pointer to the group that this player belongs to 
		Players and summoned mobs will have this filled out which is why it exists in base class */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UP4PlayerGroupObject* CurrentGroup;

	/***************************/
	/*    Public Animations    */
	/***************************/
	/* sets animation overlay enum based on current weaponstance and weapon types */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animations | Utility")
		void UpdateAnimationOverlay();

	/* used by GA_AutoAttack */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations | AutoAttack")
		UAnimMontage* MeleeRightHandAuto;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations | AutoAttack")
		UAnimMontage* MeleeLeftHandAuto;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animations | AutoAttack")
		UAnimMontage* MeleeBothHandsAuto;


	/***************************/
	/* Gameplay Ability system */  
	/***************************/

	// TODO: move this somewhere nice, gameplayabilitites need easy access though
	UFUNCTION(BlueprintCallable)
		void TryGetTarget(float Range, bool AllowEnemies, bool AllowAllies, AProject4Character*& Result);

	UFUNCTION(BlueprintCallable)
		void TryGetTargetInFront(float Range, bool AllowEnemies, bool AllowAllies, AProject4Character*& Result);


	/* Helper and general utility function to determine if actor is enemy/friendly with this caracter */
	UFUNCTION(BlueprintCallable)
		virtual bool IsEnemiesWith(AProject4Character* Other);

	UFUNCTION(BlueprintCallable)
		virtual bool IsAlliesWith(AProject4Character* Other);

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = Ability, meta = (DefaultToSelf = Target))
		class UP4BaseAttributeSet* GetAttributeSet() const;

	/* weapon stance (dual-weild, 2h, etc.) for auto attack context */
	UPROPERTY(BlueprintReadWrite, Replicated, EditDefaultsOnly, Category = "Weapons")
		EWeaponStanceType WeaponStance;

	UPROPERTY(BlueprintReadWrite, Replicated, EditDefaultsOnly, Category = "Weapons")
		EWeaponType MainHandWeaponType;

	UPROPERTY(BlueprintReadWrite, Replicated, EditDefaultsOnly, Category = "Weapons")
		EWeaponType OffHandWeaponType;


	/***************************/
	/*          Death          */
	/***************************/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float RadgollDeathDelay = 2.f;

	// Simple check if health
	UFUNCTION(BlueprintCallable)
		virtual bool IsAlive() const;

	virtual void Die();

	// blueprintcallable for anim notify
	UFUNCTION(BlueprintCallable, Category = Death)
		virtual	void FinishDying();


	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation, Category = "Utility | Multicast")
		void MulticastDeath();
	void MulticastDeath_Implementation();
	bool MulticastDeath_Validate() { return true; }

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation, Category = "Utility | Multicast")
		void MulticastRespawn();
	void MulticastRespawn_Implementation();
	bool MulticastRespawn_Validate() { return true; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Death)
		void ActivateRagdoll();


	// used by gamemode to undo ragdoll IF no death montage
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
		virtual void Respawn();
	virtual void Respawn_Implementation();
	virtual bool Respawn_Validate() { return true; }

	/***************************/
	/*     Targeting system    */
	/***************************/
	
	// Pointer to selected target, Mobs will use this too for their abilitites
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Abilities)
		AActor* SelectedTarget;

	/***************************/
	/*           UI            */
	/***************************/

	UFUNCTION(BlueprintCallable)
		class UFloatingStatusBarWidget* GetFloatingStatusBarWidget();

	UFUNCTION(BlueprintCallable)
		class UP4FloatingTextWidget* GetFloatingTextWidget();

	UFUNCTION(BlueprintCallable)
		class UWidgetComponent* GetFloatingTextWidgetComponent() { return FloatingTextWidgetComponent; }

	/***************************/
	/*         Utility         */
	/***************************/

	/* This must be called by the server, mesh will be replicated automatically */
	/* Also Overrides attack intervals with new one inside weapondata */
	UFUNCTION(BlueprintCallable, Category = "Utility | Server")
		void SetRightHandWeaponInfo(const UP4ItemWeaponObject* WeaponObject);
	UFUNCTION(BlueprintCallable, Category = "Utility | Server")
		void SetLeftHandWeaponInfo(const UP4ItemWeaponObject* WeaponObject);
	UFUNCTION(BlueprintCallable, Category = "Utility | Server")
		void ResetRightHandWeaponInfo();
	UFUNCTION(BlueprintCallable, Category = "Utility | Server")
		void ResetLeftHandWeaponInfo();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation, Category = "Utility | Multicast")
		void MulticastSetWeaponSkeletalMesh(bool IsRightHand, USkeletalMesh* SkeletalMesh, const FTransform& Transform);
	void MulticastSetWeaponSkeletalMesh_Implementation(bool IsRightHand, USkeletalMesh* SkeletalMesh, const FTransform& Transform);
	bool MulticastSetWeaponSkeletalMesh_Validate(bool IsRightHand, USkeletalMesh* SkeletalMesh, const FTransform& Transform) {  return true;  }

	void PlayStunnedAnimationMontage();

protected:

	/***************************/
	/* Gameplay Ability system */
	/***************************/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		TSubclassOf<class UFloatingStatusBarWidget> UIFloatingStatusBarClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		TSubclassOf<class UP4FloatingTextWidget> UIFloatingTextWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS)
		UDataTable* AttrDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Essential Abilities")
		class UP4GameplayAbilitySet* EssentialAbilities;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Essential GE's")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Death")
		TArray<TSubclassOf<class UGameplayEffect>> OnDeathGEs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Death")
		TArray<TSubclassOf<class UGameplayEffect>> OnRespawnGEs;

	// filled at runtime
	UPROPERTY()
		TArray<FActiveGameplayEffectHandle> OnDeathGEHandles;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS | Essential GE's")
		bool StartupEffectsApplied = false; 

	/* BP -> C++ variable interface for weapon equip GE (overrides main hand interval attribute) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapons | Essential GE's")
		TSubclassOf<UGameplayEffect> EquipWeaponMainGameplayEffect;

	/* BP -> C++ variable interface for weapon equip GE (overrides off hand interval attribute) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapons | Essential GE's")
		TSubclassOf<UGameplayEffect> EquipWeaponOffameplayEffect;

	FActiveGameplayEffectHandle WeaponMainActiveGE;
	FActiveGameplayEffectHandle WeaponOffActiveGE;

	/* helper function of trygettarget, returns true if found viable target in array*/
	bool GetClosestTargetActor(TArray<FHitResult>& Hits, float Range,  bool AllowEnemies, bool AllowFriendlies, AProject4Character*& Result);

	// Called on actorspawn ONLY servers need to call this and startupeffects
	virtual	void GiveEssentialAbilities();

	// Called on actorSpawn, GE's shouldn't be canceled unless we make skills to stop regen
	UFUNCTION(BlueprintCallable)
	virtual void AddAllStartupEffects();

	// Init playerAttributes with .csv
	virtual void InitializeAttributeSet();

	/***************************/
	/*          Death          */
	/***************************/

	UFUNCTION()
		void ApplyRespawnGameplayEffects();

	UFUNCTION()
		void ApplyDeathGameplayEffects();

	UFUNCTION()
		void RemoveDeathGameplayEffects();

	// delay from ragdoll in die() to FinishDying()
	// i.e time to show death animation before moving onto respawn timer


	FTimerHandle RadgollDeathHandle;

	// optinal death animation montage (doesnt exist -> ALS ragdoll)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UAnimMontage* StunnedMontage;

	/* Tags to bind to when granted to this char */
	FGameplayTag DeadTag;
	FGameplayTag AliveTag;
	FGameplayTag RespawnTag;
	FGameplayTag StunnedTag;

	/***************************/
	/*           UI            */
	/***************************/

	UFUNCTION(BlueprintCallable)
		virtual void InitFloatingStatusBarWidget();

	UFUNCTION(BlueprintCallable)
		virtual void InitFloatingTextWidgetComponent();

	// TODO: Find a way to fill this out
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText CharacterName;

	// Height of FloatingStatusBar = <CapsulehalfHeight> * <this Coeff> 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floating Status Bar | Height")
		float FloatingStatusBarHeightCoeff = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floating Status Bar | Cull Distance")
		float FloatingStatusBarCullDistance = 100.f;

public:
	/* Virtual Overrides */

	/* override BeginPlay */
	virtual void BeginPlay() override;
};

