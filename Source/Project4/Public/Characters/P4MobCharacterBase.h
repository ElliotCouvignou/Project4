// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Characters/Project4Character.h"
#include "GameplayEffectTypes.h"
#include "P4MobCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4MobCharacterBase : public AProject4Character
{
	GENERATED_BODY()
	
public:
	AP4MobCharacterBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Patrol ")
		TArray<FVector> PatrolPoints;

protected:

	// Hard refs of ASC and AS, unique names to not conflict with base class
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponentHardRef;
	UPROPERTY()
	class UPlayerAttributeSet* AttributeSetHardRef;


	/* Delegates */
	UFUNCTION(BlueprintCallable)
		void BindDelegates();

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;

	/* Delegate Handlers */
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);

	virtual void OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	/*  Virtual Overrides  */
	virtual void FinishDying() override;

	virtual void BeginPlay() override;

};
