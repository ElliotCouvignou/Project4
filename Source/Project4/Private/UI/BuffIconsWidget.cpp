// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/BuffIconsWidget.h"

FBuffIconDataStruct UBuffIconsWidget::CreateBuffIconDataStruct(const FGameplayTag& Bufftag, const FGameplayEffectSpec& SpecApplied)
{
	return FBuffIconDataStruct(Bufftag, SpecApplied);
}
