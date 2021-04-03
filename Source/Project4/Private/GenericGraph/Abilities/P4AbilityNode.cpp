// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GenericGraph/Abilities/P4AbilityNode.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"


#define LOCTEXT_NAMESPACE "AbilityPoolNode"

UP4AbilityNode::UP4AbilityNode()
	: Super()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UP4AbilityPoolGraph::StaticClass();

	ContextMenuName = LOCTEXT("ContextMenuName", "Ability Pool Node");
#endif
}



void UP4AbilityNode::GetLeafNodes(TArray<UP4AbilityNode*>& Result)
{
	Result.Empty();
	GetLeafNodesRecursive(Result);
}

void UP4AbilityNode::GetLeafNodesRecursive(TArray<UP4AbilityNode*>& Result)
{
    // if node is leaf node, print its data    
    if (IsLeafNode())
    {
        Result.AddUnique(this);
        return;
    }

    // not leaf, get children and go agane
	for (UGenericGraphNode* child : ChildrenNodes)
	{
		UP4AbilityNode* P4AN = Cast<UP4AbilityNode>(child);
		if (P4AN)
		{
			P4AN->GetLeafNodesRecursive(Result);
		}
	}
}

#if WITH_EDITOR



FLinearColor UP4AbilityNode::GetBackgroundColor() const
{
	UP4AbilityPoolGraph* _Graph = Cast<UP4AbilityPoolGraph>(GetGraph());

	if (_Graph)
	{
		return _Graph->AbilityPoolColor;
	}
	
	return FLinearColor::Black;
}

#endif

#undef LOCTEXT_NAMESPACE