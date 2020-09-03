// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Project4PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AProject4PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProject4PlayerState();

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = Ability, meta = (DefaultToSelf = Target))
		class UPlayerAttributeSet* GetAttributeSet() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
		bool IsAlive() const;
	
	//UFUNCTION(BlueprintCallable, Category = "PlayerState|UI")
	//	void ShowAbilityConfirmCancelText(bool ShowText);

	// Sets this ASC and attribute set refs off new character's 
	UFUNCTION(BlueprintCallable, Category = "PlayerState|Utility")
		void BindAbilityDelegates();




protected:

	// Hard refs of ASC and AS, this gets plugged into AProject4Characters' weak ptrs
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UPlayerAttributeSet* AttributeSet;
	


	FGameplayTag DeadTag;
	FGameplayTag BuffDebuffTag;

	// delegates for attribute changes
	// Only use for attributes that need to be delegated (I.E. Not all Att's need to be delegated in PS)
	FDelegateHandle BuffChangedDelegateHandle;

	/*   Progression Stat Bindings   */
	FDelegateHandle LevelChangedDelegateHandle;
	FDelegateHandle CarryWeightChangedDelegateHandle;
	FDelegateHandle MaxCarryWeightChangedDelegateHandle;

	/*   Base Stat Bindings   */
	FDelegateHandle StrengthChangedDelegateHandle;
	FDelegateHandle DexterityChangedDelegateHandle;
	FDelegateHandle IntelligenceChangedDelegateHandle;
	FDelegateHandle SpiritChangedDelegateHandle;

	/*   Resource Stat Bindings   */
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle HealthMaxChangedDelegateHandle;
	FDelegateHandle HealthRegenChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle ManaMaxChangedDelegateHandle;
	FDelegateHandle ManaRegenChangedDelegateHandle;
	FDelegateHandle EnduranceChangedDelegateHandle;
	FDelegateHandle EnduranceMaxChangedDelegateHandle;
	FDelegateHandle EnduranceRegenChangedDelegateHandle;

	/*   Defensive Stat Bindings   */
	FDelegateHandle ArmorChangedDelegateHandle;
	FDelegateHandle MagicResistanceChangedDelegateHandle;
	FDelegateHandle MovementSpeedChangedDelegateHandle;

	/*   Offsensive Stat Bindings   */
	FDelegateHandle AttackPowerDelegateHandle;
	FDelegateHandle MagicPowerChangedDelegateHandle;
	FDelegateHandle AttackSpeedChangedDelegateHandle;
	FDelegateHandle CritChanceChangedDelegateHandle;
	FDelegateHandle CritDamageChangedDelegateHandle;

	// Called on GE's applied to this PS
	void OnActiveGameplayEffectApplied(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	
	// Holds tag names that are delegated for removal
	// used to make sure we dont make 2 delegates for same purpose
	TArray<FString>DelegatedBuffTags;

	// Used by above functions for Tags with infinite duration so bind event for when they die
	// THIS MUST BE HANDLED BY SERVERS, (Tag stack counts dont get updated after 1 stack for clients. IDK weird bug)
	void OnBuffTagRemoved(const FGameplayTag Tag, int32 Count);
	void OnBuffGameplayEffectStackChanged(FActiveGameplayEffectHandle ActiveHandle, int32 NewStackCount, int32 OldStackCount);


	// Above delegate funciton handlers
	//xvirtual void OnBuffDebuffTagChanged(const FGameplayTag Tag, int32 NewCount);
	/*   Progression Stat Callback   */
	virtual void MaxCarryWeightChanged(const FOnAttributeChangeData& Data);
	virtual void CarryWeightChanged(const FOnAttributeChangeData& Data);
	virtual void LevelChanged(const FOnAttributeChangeData& Data);

	/*   Base Stat Callback   */
	virtual void StrengthChanged(const FOnAttributeChangeData& Data);
	virtual void DexterityChanged(const FOnAttributeChangeData& Data);
	virtual void IntelligenceChanged(const FOnAttributeChangeData& Data);
	virtual void SpiritChanged(const FOnAttributeChangeData& Data);

	/*   Resource Stat Callback   */
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthMaxChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void ManaMaxChanged(const FOnAttributeChangeData& Data);
	virtual void ManaRegenChanged(const FOnAttributeChangeData& Data);
	virtual void EnduranceChanged(const FOnAttributeChangeData& Data);
	virtual void EnduranceMaxChanged(const FOnAttributeChangeData& Data);
	virtual void EnduranceRegenChanged(const FOnAttributeChangeData& Data);

	/*   Defensive Stat Callback   */
	virtual void ArmorChanged(const FOnAttributeChangeData& Data);
	virtual void MagicResistanceChanged(const FOnAttributeChangeData& Data);
	virtual void MovementSpeedChanged(const FOnAttributeChangeData& Data);

	/*   Offsensive Stat Callback   */
	virtual void AttackPowerChanged(const FOnAttributeChangeData& Data);
	virtual void MagicPowerChanged(const FOnAttributeChangeData& Data);
	virtual void AttackSpeedChanged(const FOnAttributeChangeData& Data);
	virtual void CritChanceChanged(const FOnAttributeChangeData& Data);
	virtual void CritDamageChanged(const FOnAttributeChangeData& Data);

	virtual void OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
