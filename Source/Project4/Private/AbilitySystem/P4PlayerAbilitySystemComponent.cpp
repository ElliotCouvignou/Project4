// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"
#include "GenericGraph/Abilities/P4AbilityNode.h"
#include "Project4GameMode.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

UP4PlayerAbilitySystemComponent::UP4PlayerAbilitySystemComponent()
	:Super()
{
	AbilityPools = { EClassAbilityPoolType::None };
}

void UP4PlayerAbilitySystemComponent::GetLearnedPoolAbilities(TArray<TSubclassOf<UP4GameplayAbility>>& Abilities)
{
	Abilities.Empty();
	TArray<FGameplayAbilitySpec>& AA = GetActivatableAbilities();
	for (FGameplayAbilitySpec& Spec : AA)
	{
		FGameplayTagContainer Tag = Spec.Ability->AbilityTags;
		
		FGameplayTag abilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill"));
		if (Tag.HasTag(abilityTag))
		{
			Abilities.Add(Spec.Ability->GetClass());
		}
	}
}

void UP4PlayerAbilitySystemComponent::Server_OnAbilityPoolPicked_Implementation(EClassAbilityPoolType newPoolType)
{
	if (newPoolType != EClassAbilityPoolType::None)
	{
		for (EClassAbilityPoolType& Type : AbilityPools)
		{
			if (Type == EClassAbilityPoolType::None)
			{
				Type = newPoolType;
			}
		}
	}
}

void UP4PlayerAbilitySystemComponent::Server_OnPlayerSkillDropInteracted_Implementation()
{

	AProject4GameMode* P4GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	if (P4GM)
	{
		AProject4Character* P4C = Cast<AProject4Character>(GetOwner());
		AProject4Controller* PC = (P4C) ? Cast<AProject4Controller>(P4C->GetController()) : nullptr;
		if (PC)
		{
			// Roll Random Abilities: gives 3 options and get more specific/narrow from which pool you want to pick from
			int numGeneralSkillPoolChoices = 3;  // this can be opened up to other params

			UP4AbilityPoolGraph* APG = P4GM->GlobalAbilityPoolGraph;
			TArray<EAbilityCategory> GeneratedGeneralPool;
			APG->GetAbilityCategoriesFromPools(AbilityPools, GeneratedGeneralPool);

			while (GeneratedGeneralPool.Num() > numGeneralSkillPoolChoices)
			{
				GeneratedGeneralPool.RemoveAt(FMath::Rand() % GeneratedGeneralPool.Num());
			}

			// send client UI to choose 
			PC->Client_DisplaySkillDropGeneralPoolWidget(GeneratedGeneralPool);
		}
	}
}

void UP4PlayerAbilitySystemComponent::Server_OnGeneralSkillPoolSelected_Implementation(EAbilityCategory SelectedPool)
{
	// pick from pools and send resulting abilities or more ui choices to player
	TArray<UP4AbilityNode*> PoolHeadNodes;
	AProject4GameMode* P4GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	if (P4GM)
	{
		UP4AbilityPoolGraph* APG = P4GM->GlobalAbilityPoolGraph;
		if (APG)
		{
			AProject4Character* P4C = Cast<AProject4Character>(GetOwner());
			AProject4Controller* PC = (P4C) ? Cast<AProject4Controller>(P4C->GetController()) : nullptr;
			if (PC)
			{
				TArray<TSubclassOf<UP4GameplayAbility>> Abilities;
				APG->GetAbilitiesFromPoolsAndCategory(AbilityPools, SelectedPool, Abilities);
				PC->Client_DisplaypAbilityChoicesWidget(Abilities);
			}
		} 
	}


}



void UP4PlayerAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(UP4PlayerAbilitySystemComponent, AbilityPools);
	DOREPLIFETIME(UP4PlayerAbilitySystemComponent, MaxAbilityPools);
}