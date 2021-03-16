// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AI/Tasks/BTTask_WaitForAbilityCooldown.h"
#include "AI/P4AIControllerBase.h"
#include "Characters/P4MobCharacterBase.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


UBTTask_WaitForAbilityCooldown::UBTTask_WaitForAbilityCooldown(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "WaitForAbilityCooldown";
}

EBTNodeResult::Type UBTTask_WaitForAbilityCooldown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	AP4AIControllerBase* AIC = Cast<AP4AIControllerBase>(OwnerComp.GetAIOwner());
	AP4MobCharacterBase* P4Mob = Cast<AP4MobCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIC && AIC->ChaseTask && P4Mob)
	{
		FGameplayTagContainer& CDTags = P4Mob->GetCooldownContainer();
		if (!CDTags.IsEmpty()) {
			CDChangedTask = UAsyncTaskCooldownChanged::ListenForCooldownChange(P4Mob->GetAbilitySystemComponent(), CDTags, true, false);

			FScriptDelegate OnCooldownEndDelegate;
			OnCooldownEndDelegate.BindUFunction(this, "OnCooldownEnd");
			CDChangedTask->OnCooldownEnd.AddUnique(OnCooldownEndDelegate);

			print(FString("Created listen Task"));

			return EBTNodeResult::InProgress;
		}
		else
		{
			FName Name = AIC->StaticClass()->GetFName();
			print(FString("IsEmpty"));
			UE_LOG(LogTemp, Error, TEXT("%s has empty tracked CooldownTagContainer"), &Name);
		}
	}

	return NodeResult;
}

void UBTTask_WaitForAbilityCooldown::OnCooldownEnd(FGameplayTag CooldownTag, float TimeRemaining, float Duration)
{
	print(FString("OnCooldownEnd() Tag: " + CooldownTag.GetTagName().ToString()));
	CDChangedTask->EndTask();
	FinishLatentTask(*Cast<UBehaviorTreeComponent>(GetOuter()), EBTNodeResult::Succeeded);
}
