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
	
	
	/* Use patrol point actor if you want to visually set markers, you can still fill this out
	   Yourself if needed*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Patrol ")
		class APatrolPointsActor* PatrolPointsActor;
	/* filled out if there is a PatrolPointActor */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Patrol ", Meta = (MakeEditWidget = true))
		TArray<FVector> PatrolPoints;


	/* Used in AI behavior tree to tell if we are in range to attack, this represents range for both melee and ranged */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Stats ")
		float AttackRange;

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
