// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4GameplayAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/P4GameplayAbility.h"

UP4GameplayAbilitySet::UP4GameplayAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UP4GameplayAbilitySet::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const
{
	for (const FP4GameplayAbilityBindInfo& BindInfo : Abilities)
	{
		if (BindInfo.AbilityClass)
		{
			int32 thing = (int32)BindInfo.Command;
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.AbilityClass, 1, thing));
		}
	}
}

void UP4GameplayAbilitySet::AddClassesToRemove(TArray<FGameplayAbilitySpecHandle>& RemoveArray, TArray<FGameplayAbilitySpec>& AbilitiesArray)
{
	
}

//bool FP4GameplayAbilityBindInfo::operator==(const UGameplayAbility& lhs)
//{
//	return lhs == AbilityClass.GetDefaultObject();
//}

