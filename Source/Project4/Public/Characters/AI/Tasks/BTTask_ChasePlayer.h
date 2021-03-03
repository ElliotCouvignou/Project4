// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class PROJECT4_API UBTTask_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector TargetActor;
	
};
