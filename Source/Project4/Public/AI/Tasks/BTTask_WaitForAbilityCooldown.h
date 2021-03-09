// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AbilitySystem/Tasks/AsyncTaskCooldownChanged.h"
#include "BTTask_WaitForAbilityCooldown.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UBTTask_WaitForAbilityCooldown : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()


private:

	UAsyncTaskCooldownChanged* CDChangedTask;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OnCooldownEnd(FGameplayTag CooldownTag, float TimeRemaining, float Duration);
	
};
