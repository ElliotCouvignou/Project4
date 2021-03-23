// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "GenericGraph/Abilities/P4AbilityPoolFactory.h"
#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"


#define LOCTEXT_NAMESPACE "AbilityPoolFactory"

UP4AbilityPoolFactory::UP4AbilityPoolFactory()
	:Super()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UP4AbilityPoolGraph::StaticClass();
}

UObject* UP4AbilityPoolFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

FText UP4AbilityPoolFactory::GetDisplayName() const
{
	return LOCTEXT("FactoryName", "Ability Pool");
}

FString UP4AbilityPoolFactory::GetDefaultNewAssetName() const
{
	return "AbilityPool";
}

#undef LOCTEXT_NAMESPACE
