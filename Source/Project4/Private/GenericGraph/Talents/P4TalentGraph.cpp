// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GenericGraph/Talents/P4TalentGraph.h"
#include "GenericGraph/Talents/P4TalentGraphNode.h"
#include "GenericGraph/Talents/P4TalentGraphEdge.h"

#define LOCTEXT_NAMESPACE "TalentGraph"

UP4TalentGraph::UP4TalentGraph()
	: Super()
{
	NodeType = UP4TalentGraphNode::StaticClass();
	EdgeType = UP4TalentGraphEdge::StaticClass();

	TalentGraphColor = FLinearColor::Black;

	Name = "Talent Graph";
}

#undef LOCTEXT_NAMESPACE