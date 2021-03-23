// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GenericGraph/Talents/P4TalentGraphNode.h"
#include "GenericGraph/Talents/P4TalentGraph.h"

#define LOCTEXT_NAMESPACE "DialogueSessionNode"

UP4TalentGraphNode::UP4TalentGraphNode()
    : Super()
{
#if WITH_EDITORONLY_DATA
    CompatibleGraphType = UP4TalentGraph::StaticClass();

    ContextMenuName = LOCTEXT("ConextMenuName", "Talent Graph Node");
#endif
}

#if WITH_EDITOR

FText UP4TalentGraphNode::GetNodeTitle() const
{
    return TalentName.IsEmpty() ? LOCTEXT("EmptyParagraph", "(Empty paragraph)") : TalentName;
}

void UP4TalentGraphNode::SetNodeTitle(const FText& NewTitle)
{
    TalentName = NewTitle;
}

FLinearColor UP4TalentGraphNode::GetBackgroundColor() const
{
    //UDialogueSession* Graph = Cast<UDialogueSession>(GetGraph());
    //
    //if (Graph == nullptr)
    //    return Super::GetBackgroundColor();
    //
    //switch (DialoguerPostion)
    //{
    //case EDialoguerPostion::Left:
    //    return Graph->LeftDialoguerBgColor;
    //case EDialoguerPostion::Right:
    //    return Graph->RightDialoguerBgColor;
    //default:
    //    return FLinearColor::Black;
    //}
    return FLinearColor::Black;
}

#endif

#undef LOCTEXT_NAMESPACE