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

	// Weak Reference to character's ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UPlayerAttributeSet* AttributeSet;
	
	UPROPERTY()
	FGameplayTag DeadTag;

	// delegates for attribute changes
	// Only use for attributes that need to be delegated (I.E. Not all Att's need to be delegated in PS)
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle HealthMaxChangedDelegateHandle;
	FDelegateHandle HealthRegenChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle ManaMaxChangedDelegateHandle;
	FDelegateHandle ManaRegenChangedDelegateHandle;
	FDelegateHandle EnduranceChangedDelegateHandle;
	FDelegateHandle EnduranceMaxChangedDelegateHandle;
	FDelegateHandle EnduranceRegenChangedDelegateHandle;
	
	// Above delegate funciton handlers
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthMaxChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void ManaMaxChanged(const FOnAttributeChangeData& Data);
	virtual void ManaRegenChanged(const FOnAttributeChangeData& Data);
	virtual void EnduranceChanged(const FOnAttributeChangeData& Data);
	virtual void EnduranceMaxChanged(const FOnAttributeChangeData& Data);
	virtual void EnduranceRegenChanged(const FOnAttributeChangeData& Data);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
