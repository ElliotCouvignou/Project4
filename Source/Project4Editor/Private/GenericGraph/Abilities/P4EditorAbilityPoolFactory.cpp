// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GenericGraph/Abilities/P4EditorAbilityPoolFactory.h"
#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"


#define LOCTEXT_NAMESPACE "EditorAbilityPoolFactory"

UP4EditorAbilityPoolFactory::UP4EditorAbilityPoolFactory()
	:Super()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UP4AbilityPoolGraph::StaticClass();
}

UObject* UP4EditorAbilityPoolFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

FText UP4EditorAbilityPoolFactory::GetDisplayName() const
{
	return LOCTEXT("FactoryName", "Editor Ability Pool");
}

FString UP4EditorAbilityPoolFactory::GetDefaultNewAssetName() const
{
	return "EditorAbilityPool";
}

#undef LOCTEXT_NAMESPACE
