// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/SkillTreeComponent.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "AbilitySystem/P4GameplayAbility.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

// Sets default values for this component's properties
USkillTreeComponent::USkillTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	ActiveSecondarySkillTreeIndex = 0;
	ActiveMainSkillTreeIndex = 0;
}


void USkillTreeComponent::ServerTryRankUpSkill_Implementation(int SkillTreeIndex, bool IsMainTree)
{
	// TODO: check if player is in combat but that detection aint in yet...
	// Get Stuff based on bool param
	TArray<FSkillTreeNodeStruct>& SkillTree = (IsMainTree) ? MainSkillTree : SecondarySkillTree;
	int& PointsToSpend = (IsMainTree) ? MainSkillTreePoints : SecondarySkillTreePoints;
	int ActiveIndex = (IsMainTree) ? ActiveMainSkillTreeIndex : ActiveSecondarySkillTreeIndex;
	TArray<int>& ChangedIndexesArray = (IsMainTree) ? ChangedMainSkillTreeNodes : ChangedSecondarySkillTreeNodes;
	
	// Check: Valid data asset, valid index, and has any points to spend
	if (SkillTree.Num() > SkillTreeIndex && PointsToSpend > 0)
	{
		bool successful = false;
		FSkillTreeNodeStruct& CurNode = SkillTree[SkillTreeIndex];
		FSkillTreeNodeDataAssetStruct& CurNodeAsset = UseableSkillTrees[ActiveIndex]->SkillTreeNodes[SkillTreeIndex];
	
		if (CurNode.CurrentRank < CurNodeAsset.MaxRank)
		{
			AP4PlayerCharacterBase* Char = Cast<AP4PlayerCharacterBase>(GetOwner());
			UAbilitySystemComponent* ASC = (Char) ? Char->GetAbilitySystemComponent() : nullptr;
			if (!ASC) {
				return;
			}
	
			if (CurNode.CurrentRank == 0)
			{
				// Learn Ability
				CurNode.SpecHandle = ASC->GiveAbility(FGameplayAbilitySpec(CurNodeAsset.AbilityClass, 1));
				successful = true;
			}
			else
			{
				// rank up ability
				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(CurNode.SpecHandle);
				if (Spec)
				{
					Spec->Level += 1;
					successful = true;
				}
			}
			if (successful)
			{
				CurNode.CurrentRank += 1;
				PointsToSpend -= 1;
				
				// TODO: find way to detect if we are in listen server and host wants to rank up
				//			as this array delegation is only meant for this case
				TArray<int> ForListenHostCase = { SkillTreeIndex };
				(IsMainTree) ? OnMainTreeNodeUpdated.Broadcast(ForListenHostCase) : OnSecondaryTreeNodeUpdated.Broadcast(ForListenHostCase);
				ClientSkillTreeNodeUpdateDelegate(SkillTreeIndex, IsMainTree);
			}
		}
	}
}

void USkillTreeComponent::ServerTryRankDownSkill_Implementation(int SkillTreeIndex, bool IsMainTree)
{
	// Get Stuff based on bool param
	TArray<FSkillTreeNodeStruct>& SkillTree = (IsMainTree) ? MainSkillTree : SecondarySkillTree;
	int& PointsToSpend = (IsMainTree) ? MainSkillTreePoints : SecondarySkillTreePoints;
	int ActiveIndex = (IsMainTree) ? ActiveMainSkillTreeIndex : ActiveSecondarySkillTreeIndex;
	TArray<int>& ChangedIndexesArray = (IsMainTree) ? ChangedMainSkillTreeNodes : ChangedSecondarySkillTreeNodes;
	
	// Check: Valid data asset, valid index, and has any points to spend
	if (SkillTree.Num() > SkillTreeIndex && PointsToSpend > 0)
	{
		bool successful = false;
		FSkillTreeNodeStruct& CurNode = SkillTree[SkillTreeIndex];

		if (CurNode.CurrentRank > 0)
		{
			AP4PlayerCharacterBase* Char = Cast<AP4PlayerCharacterBase>(GetOwner());
			UAbilitySystemComponent* ASC = (Char) ? Char->GetAbilitySystemComponent() : nullptr;
			if (!ASC) {
				return;
			}
	
			if (CurNode.CurrentRank == 1)
			{
				// Unlearn Ability
				ASC->ClearAbility(CurNode.SpecHandle);
				successful = true;
			}
			else
			{
				// rank up ability
				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(CurNode.SpecHandle);
				if (Spec)
				{
					Spec->Level -= 1;
					successful = true;
				}
			}
			if (successful)
			{
				CurNode.CurrentRank -= 1;
				PointsToSpend += 1;

				// TODO: find way to detect if we are in listen server and host wants to rank up
				//			as this array delegation is only meant for this case
				TArray<int> ForListenHostCase = {SkillTreeIndex};
				(IsMainTree) ? OnMainTreeNodeUpdated.Broadcast(ForListenHostCase) : OnSecondaryTreeNodeUpdated.Broadcast(ForListenHostCase);
				ClientSkillTreeNodeUpdateDelegate(SkillTreeIndex, IsMainTree);
			}
		}
	}
}

void USkillTreeComponent::ClientSkillTreeNodeUpdateDelegate_Implementation(int Index, bool IsMainTree)
{
	(IsMainTree) ? ChangedMainSkillTreeNodes.Add(Index) : ChangedSecondarySkillTreeNodes.Add(Index);
}

void USkillTreeComponent::GetAssetNodeStruct(const int NodeIndex, const bool IsMainTree, FSkillTreeNodeDataAssetStruct& DataAssetNodeStruct)
{
	int ActiveIndex = (IsMainTree) ? ActiveMainSkillTreeIndex : ActiveSecondarySkillTreeIndex;
	DataAssetNodeStruct = UseableSkillTrees[ActiveIndex]->SkillTreeNodes[NodeIndex];
}

void USkillTreeComponent::InitSkillTreeFromDataAsset()
{
	AP4PlayerCharacterBase* Char = Cast<AP4PlayerCharacterBase>(GetOwner());

	if (MainSkillTreeDataAsset)
	{
		MainSkillTree.Empty();

		int idx = 0;
		for (FSkillTreeNodeDataAssetStruct& NodeStruct : MainSkillTreeDataAsset->SkillTreeNodes)
		{
			MainSkillTree.Add(FSkillTreeNodeStruct(NodeStruct, idx));
			idx++;
			
		}
	}
	if (SecondarySkillTreeDataAsset)
	{
		SecondarySkillTree.Empty();

		int idx = 0;
		for (FSkillTreeNodeDataAssetStruct& NodeStruct : SecondarySkillTreeDataAsset->SkillTreeNodes)
		{
			SecondarySkillTree.Add(FSkillTreeNodeStruct(NodeStruct, idx));
			idx++;
		}
	}
}

void USkillTreeComponent::OnRep_MainSkillTree()
{
	OnMainTreeNodeUpdated.Broadcast(ChangedMainSkillTreeNodes);
	ChangedMainSkillTreeNodes.Empty();
} 

void USkillTreeComponent::OnRep_SecondarySkillTree()
{
	OnSecondaryTreeNodeUpdated.Broadcast(ChangedSecondarySkillTreeNodes);
	ChangedSecondarySkillTreeNodes.Empty();
}

// Called when the game starts
void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitSkillTreeFromDataAsset();
}

void USkillTreeComponent::ClearChangedSkillTreeNodes()
{
	ChangedMainSkillTreeNodes.Empty();
	ChangedSecondarySkillTreeNodes.Empty();
}

void USkillTreeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(USkillTreeComponent, MainSkillTreePoints, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USkillTreeComponent, SecondarySkillTreePoints, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USkillTreeComponent, MainSkillTree, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USkillTreeComponent, SecondarySkillTree, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USkillTreeComponent, ActiveMainSkillTreeIndex, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USkillTreeComponent, ActiveSecondarySkillTreeIndex, COND_OwnerOnly);

}