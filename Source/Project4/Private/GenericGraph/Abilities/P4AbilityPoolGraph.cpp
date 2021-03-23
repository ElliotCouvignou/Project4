// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"
#include "GenericGraph/Abilities/P4AbilityEdge.h"

#define LOCTEXT_NAMESPACE "AbilityPool"

UP4AbilityPoolGraph::UP4AbilityPoolGraph()
	: Super()
{
	NodeType = UP4AbilityNode::StaticClass();
	EdgeType = UP4AbilityEdge::StaticClass();

	AbilityPoolColor = FLinearColor::Black;

	Name = "Ability Pool";
}

#undef LOCTEXT_NAMESPACE

void UP4AbilityPoolGraph::GetAbilityCategoriesFromPools(TArray<EClassAbilityPoolType>& AbilityPools, TArray<EAbilityCategory>& Result)
{
	Result.Empty();
	for (UGenericGraphNode* PoolNode : RootNodes)
	{
		UP4AbilityNode* P4AN = Cast<UP4AbilityNode>(PoolNode);
		if (P4AN && P4AN->NodeType == EP4AbilityNodeType::Pool && AbilityPools.Contains(P4AN->PoolType))
		{
			// In related poolclass, loook through lvl 1 children and add valid category types
			for (UGenericGraphNode* ChildNode : P4AN->ChildrenNodes)
			{
				UP4AbilityNode* P4AN_c = Cast<UP4AbilityNode>(ChildNode);
				if (P4AN_c && P4AN_c->NodeType == EP4AbilityNodeType::Category)
				{
					Result.AddUnique(P4AN_c->CategoryType);
				}
			}
		}
	}
}

void UP4AbilityPoolGraph::GetAbilitiesFromPoolsAndCategory(TArray<EClassAbilityPoolType>& AbilityPools, EAbilityCategory Category, TArray<TSubclassOf<UP4GameplayAbility>>& Result)
{
	Result.Empty();
	for (UGenericGraphNode* PoolNode : RootNodes)
	{
		UP4AbilityNode* P4AN = Cast<UP4AbilityNode>(PoolNode);
		if (P4AN && P4AN->NodeType == EP4AbilityNodeType::Pool && AbilityPools.Contains(P4AN->PoolType))
		{
			TArray<UP4AbilityNode*> LeafNodes;
			P4AN->GetLeafNodes(LeafNodes);

			for (UP4AbilityNode* Leaf : LeafNodes)
			{
				if (Leaf->NodeType == EP4AbilityNodeType::Ability)
				{
					Result.AddUnique(Leaf->Ability);
				}
			}
		}
	}
}
