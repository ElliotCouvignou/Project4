// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4AbilityModifier.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

UP4AbilityModifierInfo::UP4AbilityModifierInfo()
{

}

UP4AbilityModifierInfo::~UP4AbilityModifierInfo()
{
}

UP4AbilityModifierInfo::UP4AbilityModifierInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//CalculateModifierMagnitudes(ModifierMagnitudes);
	OnModifierGranted();
}


void UP4AbilityModifierInfo::CalculateModifierMagnitudes_Implementation(TArray<float>& Results)
{

}


void UP4AbilityModifierInfo::OnModifierGranted_Implementation()
{

}


void UP4AbilityModifierInfo::OnModifierMagnitudesChanged_Implementation()
{

}
