// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interactables/P4ItemWeaponObject.h"
#include "NiagaraComponent.h"
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

	// delay from ragdoll in die() to FinishDying()
	// i.e time to show death animation before moving onto respawn timer
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float RadgollDeathDelay = 2.f;

	FTimerHandle RadgollDeathHandle;

	// optinal death animation montage (doesnt exist -> ALS ragdoll)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UAnimMontage* StunnedMontage;

	/* Tags to bind to when granted to this char */
	FGameplayTag DeadTag;
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

