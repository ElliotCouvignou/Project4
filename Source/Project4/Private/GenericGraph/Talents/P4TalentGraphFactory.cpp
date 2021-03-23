// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GenericGraph/Talents/P4TalentGraphFactory.h"
#include "GenericGraph/Talents/P4TalentGraph.h"

#define LOCTEXT_NAMESPACE "TalentGraphFactory"

UP4TalentGraphFactory::UP4TalentGraphFactory()
	: Super()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UP4TalentGraph::StaticClass();
}

UObject* UP4TalentGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

FText UP4TalentGraphFactory::GetDisplayName() const
{
	return LOCTEXT("FactoryName", "Talent Graph");
}

FString UP4TalentGraphFactory::GetDefaultNewAssetName() const
{
	return "TalentGraph";
}

#undef LOCTEXT_NAMESPACE