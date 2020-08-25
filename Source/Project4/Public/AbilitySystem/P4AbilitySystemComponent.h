// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/P4GameplayAbility.h"

#include "P4AbilitySystemComponent.generated.h"

class AProject4Character;


/** Notify interested parties that ActiveGameplayEffect has been modified */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnActiveGameplayEffectDurationChanged, const FActiveGameplayEffect&);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoAttackHitTargets, const TArray<AProject4Character*>&, HitTargets);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAutoAttackHit);


/**
 * 
 */
UCLASS()
class PROJECT4_API UP4AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	/*****************/
	/*   Delegates   */
	/*****************/

	FOnActiveGameplayEffectDurationChanged ActiveGameplayEffectDirtiedCallback;

	/* contains array of all hit actors */
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnAutoAttackHitTargets AutoAttackHitTargetsCallback;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnAutoAttackHit AutoAttackHitCallback;

	UFUNCTION(BlueprintCallable, Category = "Delegates")
		void BroadcastAutoAttackResults(const TArray<AProject4Character*>& HitArray);

	/****************************/
	/*   BP Exposed Functions   */
	/****************************/
	
	/** Checks if the ability system is currently blocking InputID. Returns true if InputID is blocked, false otherwise.  */
	UFUNCTION(BlueprintCallable, Category = "BP Exposed")
		bool IsAbilityInputBlocked_BP(int32 InputID) const { return IsAbilityInputBlocked(InputID); }
	
	/** Block or cancel blocking for specific input IDs */
	UFUNCTION(BlueprintCallable, Category = "BP Exposed")
		void BlockAbilityByInputID_BP(int32 InputID) { BlockAbilityByInputID(InputID); }
	UFUNCTION(BlueprintCallable, Category = "BP Exposed")
		void UnBlockAbilityByInputID_BP(int32 InputID) { UnBlockAbilityByInputID(InputID); }

	/** Block or cancel blocking for specific ability tags */
	UFUNCTION(BlueprintCallable, Category = "BP Exposed")
		void BlockAbilitiesWithTags_BP(const FGameplayTagContainer& Tags) { BlockAbilitiesWithTags(Tags); }
	UFUNCTION(BlueprintCallable, Category = "BP Exposed")
		void UnBlockAbilitiesWithTags_BP(const FGameplayTagContainer& Tags) { UnBlockAbilitiesWithTags(Tags); }

	/** Checks if the ability system is currently blocking InputID. Returns true if InputID is blocked, false otherwise.  */
//UFUNCTION(BlueprintCallable, Category = "BP Exposed")
//	bool IsAbilityInputBlocked_BP(int32 InputID) const { return IsAbilityInputBlocked(InputID); }
//
///** Block or cancel blocking for specific input IDs */
//
///* Client Request input block (server needs to execute) */
//UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "BP Exposed")
//	void ServerBlockAbilityByInputID(int32 InputID);
//void ServerBlockAbilityByInputID_Implementation(int32 InputID) { BlockAbilityByInputID(InputID); }
//bool ServerBlockAbilityByInputID_Validate(int32 InputID) { return true; }
//
///* Client Request input unblock (server needs to execute) */
//UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "BP Exposed")
//	void ServerUnBlockAbilityByInputID(int32 InputID);
//void ServerUnBlockAbilityByInputID_Implementation(int32 InputID) { UnBlockAbilityByInputID(InputID); }
//bool ServerUnBlockAbilityByInputID_Validate(int32 InputID) { return true; }
//
/** Block or cancel blocking for specific ability tags */
//UFUNCTION(BlueprintCallable, Category = "BP Exposed")
//	void BlockAbilitiesWithTags_BP(const FGameplayTagContainer& Tags) { BlockAbilitiesWithTags(Tags); }
//UFUNCTION(BlueprintCallable, Category = "BP Exposed")
//	void UnBlockAbilitiesWithTags_BP(const FGameplayTagContainer& Tags) { UnBlockAbilitiesWithTags(Tags); }


	/* Virtual Overrides */

	/* Calls palyMontage ASC function that isn't blueprint exposed on default
		Similar function exists in P4GameplayAbility with a target actor
				OUTDATED FUNCTION????										*/
	UFUNCTION(BlueprintCallable)
		virtual float PlayMontage(UGameplayAbility* AnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* Montage, float InPlayRate, FName StartSectionName = NAME_None) override;

	
	
	// Used for changing active GE durations, Used for CD refunding
	bool SetGameplayEffectDurationWithHandle(const FActiveGameplayEffectHandle& Handle, float NewDuration);
};
