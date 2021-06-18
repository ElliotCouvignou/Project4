// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Characters/Project4Character.h"
#include "Containers/SortedMap.h"
#include "Tasks/AITask_MoveTo.h"

#include "P4AIControllerBase.generated.h"


USTRUCT(BlueprintType)
struct FThreatTarget
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AProject4Character* Target;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Threat;

	FORCEINLINE bool operator==(const FThreatTarget& rhs) const
	{
		return rhs.Target == Target;
	}

	FThreatTarget()
	{
		Target = nullptr;
		Threat = 0.f;
	}

	FThreatTarget(AProject4Character* _Target, float _Threat)
	{
		Target = _Target;
		Threat = _Threat;
	}
};

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4AIControllerBase : public AAIController, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:

	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Perception")
		class UAIPerceptionComponent* AIPerceptionComponent;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Behavior")
		class UBehaviorTreeComponent* BehaviorTreeComponent;

	/* Sorted Array of struct containing a threat value and the actor associated with it
		Highest threat sorted at 0 index, least threat at last index */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Threat")
		TArray<FThreatTarget> ThreatArray;

	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Threat")
	//	TMap<AProject4Character*, int> ThreatMap;

public:


	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); }


	/* Radius around mob to notify and make them also start combat if with in radius and not already in combat */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Threat")
		float CombatNotifyRadius = 15*52.5;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Threat")
		UAITask_MoveTo* ChaseTask;



	/* returns highest threat in threatarray, if empty return nullptr */
	UFUNCTION(BlueprintCallable, Category = "Threat")
		AProject4Character* GetHighestThreatTarget();

	UFUNCTION(BlueprintCallable, Category = "Threat")
		void AddUniqueThreatActor(AProject4Character* Actor, float Threat);

	/* Adds threat to actor in threatArray */
	UFUNCTION(BlueprintCallable, Category = "Threat")
		void AddThreatToThreatArray(AProject4Character* Actor, float ThreatToAdd);

	/* Use this to effectively 'lose aggro' */
	UFUNCTION(BlueprintCallable, Category = "Threat")
		void RemoveActorFromThreatArray(AProject4Character* Actor);

	UFUNCTION(BlueprintCallable, Category = "Threat")
		void PrintThreatArray();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		 void StopFightingTarget(AProject4Character* Actor);
	virtual void StopFightingTarget_Implementation(AProject4Character* Actor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Essential")
		void MobDamageTaken(AActor* Source, float DamageTaken);

	UFUNCTION(BlueprintCallable)
		void GetThreatArray(TArray<FThreatTarget>& Result) { Result = ThreatArray; }

	UFUNCTION(BlueprintCallable)
		void OnCombatStarted(AProject4Character* Actor);

	UFUNCTION(BlueprintCallable)
		void OnCombatEnded(AProject4Character* Actor);

	UFUNCTION(BlueprintCallable)
		UBehaviorTreeComponent* GetBehaviorTreeComponent() { return BehaviorTreeComponent; }

	AP4AIControllerBase(const class FObjectInitializer& ObjectInitializer);

protected:

	UFUNCTION(BlueprintCallable)
		void NotifyNearbyFirendsOfNewThreat(AProject4Character* Actor);


};
