// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/AI/Tasks/BTTask_ChasePlayer.h"
#include "Characters/AI/P4AIControllerBase.h"
#include "Tasks/AITask_MoveTo.h"




UBTTask_ChasePlayer::UBTTask_ChasePlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	AP4AIControllerBase* MyController = Cast<AP4AIControllerBase>(OwnerComp.GetAIOwner());
	if (MyController && MyController->ChaseTask && TargetActor.GetSelectedKeyID())
	{
		
	}
	
	return NodeResult;
}
