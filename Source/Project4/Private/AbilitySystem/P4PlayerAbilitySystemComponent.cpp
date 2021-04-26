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

void UP4PlayerAbilitySystemComponent::TestModiftyAbility(TSubclassOf<UP4GameplayAbility> SelectedAbility)
{
	if (SelectedAbility)
	{
		print(FString("Start test"));

		FGameplayAbilitySpec* testspec = this->FindAbilitySpecFromClass(SelectedAbility);
		if (testspec)
		{
			UP4GameplayAbility* p4gpa = Cast<UP4GameplayAbility>(testspec->Ability);
			if (p4gpa)
			{
				p4gpa->testfloat = 1.f;
				print(FString("set testfloat to 1,.f"));
			}
		}
	}
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
		AProject4Character* P4C = Cast<AProject4Character>(GetOwnerActor());
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
			//print(FString("Client display general pools"));
			PC->Client_DisplaySkillDropGeneralPoolWidget(GeneratedGeneralPool);
		}
	}
}

void UP4PlayerAbilitySystemComponent::Server_OnGeneralSkillPoolSelected_Implementation(EAbilityCategory SelectedPool)
{
	//print(FString("Server_OnGeneralSkillPoolSelected_Implementation"));
	// pick from pools and send resulting abilities or more ui choices to player
	TArray<UP4AbilityNode*> PoolHeadNodes;
	AProject4GameMode* P4GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	if (P4GM)
	{
		UP4AbilityPoolGraph* APG = P4GM->GlobalAbilityPoolGraph;
		if (APG)
		{
			AProject4Character* P4C = Cast<AProject4Character>(GetOwnerActor());
			AProject4Controller* PC = (P4C) ? Cast<AProject4Controller>(P4C->GetController()) : nullptr;
			if (PC)
			{
				TArray<TSubclassOf<UP4GameplayAbility>> Abilities;
				APG->GetAbilitiesFromPoolsAndCategory(AbilityPools, SelectedPool, Abilities);

				TArray<TSubclassOf<UP4GameplayAbility>> LearnedAbilities;
				GetLearnedPoolAbilities(LearnedAbilities);

				// remove learnedailties from abilities
				for (TSubclassOf<UP4GameplayAbility> Ability : LearnedAbilities)
				{
					Abilities.Remove(Ability);
				}

				int numAbilityChoices = 3;  // this can be opened up to other params
				while (Abilities.Num() > numAbilityChoices)
				{
					Abilities.RemoveAt(FMath::Rand() % Abilities.Num());
				}
				
				PC->Client_DisplaypAbilityChoicesWidget(Abilities);
			}
		} 
	}
	

}

void UP4PlayerAbilitySystemComponent::Server_OnAbilityChoiceSelected_Implementation(TSubclassOf<UP4GameplayAbility> SelectedAbility)
{
	if (SelectedAbility)
	{
		//print(FString("Gave ability"));
		GiveAbility(FGameplayAbilitySpec(SelectedAbility, 1));
	}
}

void UP4PlayerAbilitySystemComponent::Server_OnPlayerSkillModifierDropInteracted_Implementation()
{

	AProject4GameMode* P4GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	if (P4GM)
	{
		AProject4Character* P4C = Cast<AProject4Character>(GetOwnerActor());
		AProject4Controller* PC = (P4C) ? Cast<AProject4Controller>(P4C->GetController()) : nullptr;
		if (PC)
		{		
			// Get Abilitites that Player has learned and can be Modified
			UP4AbilityPoolGraph* APG = P4GM->GlobalAbilityPoolGraph;
			TArray<UP4AbilityNode*> Abilities;
			TArray<TSubclassOf<UP4GameplayAbility>> LearnedAbilities;
			GetLearnedPoolAbilities(LearnedAbilities);
			APG->GetLearnedAbilityNodesFromPools(AbilityPools, LearnedAbilities, Abilities);

			// trim abilities without modifiers
			TArray<UP4AbilityNode*> AbilityNodesToRemove;
			for (UP4AbilityNode* Node : Abilities)
			{
				if (Node->AbilityModifiers.Num() <= 0)
					AbilityNodesToRemove.Add(Node);
			}

			for (UP4AbilityNode* Node : AbilityNodesToRemove)
			{
				Abilities.Remove(Node);
			}

			// from array of abilities with modifiers, choose N amount
			// TODO: figure out N (Currently N = 3)
			TArray<TSubclassOf<UP4GameplayAbility>> Result;
			while(Result.Num() < 3 && Abilities.Num() > 0)
			{
				int randidx = FMath::RandRange(0.f, (float)Abilities.Num());
				Result.Add(Abilities[randidx]->Ability);
				Abilities.RemoveAt(randidx);
			}

			if(Result.Num() > 0)
				PC->Client_DisplaypAbilityModifierAbilityChoicesWidget(Result);
			else
			{
				// TODO: print something to user
			}
		}
	}
}

void UP4PlayerAbilitySystemComponent::Server_OnAbilityModifierAbilityChoiceSelected_Implementation(TSubclassOf<UP4GameplayAbility> SelectedAbility)
{
	AProject4GameMode* P4GM = Cast<AProject4GameMode>(GetWorld()->GetAuthGameMode());
	UP4AbilityPoolGraph* APG = P4GM->GlobalAbilityPoolGraph;
	if (P4GM && APG && SelectedAbility)
	{
		AProject4Character* P4C = Cast<AProject4Character>(GetOwnerActor());
		AProject4Controller* PC = (P4C) ? Cast<AProject4Controller>(P4C->GetController()) : nullptr;
		if (PC)
		{
			// Get AbilityNodes of this ability class (gathered from all pools and overlapping categories
			TArray<UP4AbilityNode*> AbilityNodes;
			APG->GetAbilityNodes(AbilityPools, SelectedAbility, AbilityNodes);

			// generate one array of abilitymodifiers for this abilityclass
			// Tally up weights as well for rolling N Modifiers
			TMap<FGameplayTag, FP4AbilityModifierBaseInfoStruct> AbilityModifierInfos;
			for (UP4AbilityNode* Node : AbilityNodes)
			{
				AbilityModifierInfos.Append(Node->AbilityModifiers);
				//TArray<FP4AbilityModifierBaseInfoStruct> Infos;
				//Node->AbilityModifiers.GenerateValueArray(Infos);
			}

			// from this array of modifiers, choose N Amount
			// TODO: figure out N (Currently N = 3)
			TArray<FP4AbilityModifierBaseInfoStruct> Values;
			AbilityModifierInfos.GenerateValueArray(Values);
			float TotalWeight = 0.f;
			for (FP4AbilityModifierBaseInfoStruct& info : Values)
			{
				TotalWeight += info.Weight;
			}

			TArray<FP4AbilityModifierInfoStruct> Result;
			while (Result.Num() < 3 && Values.Num() > 0)
			{
				// Get Random Key Based on Weighted range
				FP4AbilityModifierInfoStruct GeneratedInfo;
				int idx = 0;
				float RandWeight = FMath::RandRange(0.f, TotalWeight);
				while (idx < Values.Num())
				{
					RandWeight -= Values[idx].Weight;
					if (RandWeight <= 0.f)
					{
						GeneratedInfo = Values[idx];	
						TotalWeight -= Values[idx].Weight;
						Values.RemoveAt(idx);
						break; // break out of this while (found out key for this iter)
						
					}
					idx++;
				}
				
				// roll ability magnitude 
				// TODO: Scale magnitude to player's current Aability modifier magnitude 
				GeneratedInfo.ModifierMagnitude = FMath::RandRange(0.f, 1.f);

				// TODO: determine rank from magnitude
				GeneratedInfo.AbilityModifierRank = (GeneratedInfo.ModifierMagnitude > 0.66f) ? EAbilityModifierRank::Epic : EAbilityModifierRank::Rare;
				GeneratedInfo.AbilityModifierRank = (GeneratedInfo.ModifierMagnitude > 0.33f) ? GeneratedInfo.AbilityModifierRank : EAbilityModifierRank::Common;

				if (!GeneratedInfo.AbilityModifierIcon)
					GeneratedInfo.AbilityModifierIcon = SelectedAbility.GetDefaultObject()->AbilityIcon;

				Result.Add(GeneratedInfo);
			}

			PC->Client_DisplaypAbilityModifierChoicesWidget(Result);
		}
	}
}

void UP4PlayerAbilitySystemComponent::Server_OnPlayerAbilityModifierSelected_Implementation(TSubclassOf<UP4GameplayAbility> AbilityClass, const FP4AbilityModifierInfoStruct& ModifierInfo)
{
	// Ability Modifiers are setup so that all you need to enable them is to grant tag and store modifier magnitude in ASC
	if (!AbilityClass)
		return;

	if (AbilityModifiers.Contains(AbilityClass))
	{
		// Add onto existing key
		//
		FP4AbilityModifierInfoMapStruct* Elem = AbilityModifiers.Find(AbilityClass);
		if (Elem->AbilityModToInfoMap.Contains(ModifierInfo.GrantedTag))
		{
			// Modifier already exists so override if magnitude is greater (should always be the case once magnitudes are scalled off current values if possible)
			FP4AbilityModifierInfoStruct* ElemInfo = Elem->AbilityModToInfoMap.Find(ModifierInfo.GrantedTag);
			ElemInfo->ModifierMagnitude = FMath::Max(ElemInfo->ModifierMagnitude, ModifierInfo.ModifierMagnitude);
		}
		else
		{
			// first time player is learning this ability modifier, create entry inn modtotinfo map
			FP4AbilityModifierInfoMapStruct NewElemMap;
			NewElemMap.AbilityModToInfoMap.Add(TTuple<FGameplayTag, FP4AbilityModifierInfoStruct>(ModifierInfo.GrantedTag, ModifierInfo));
			AddLooseGameplayTag(ModifierInfo.GrantedTag);
			bIsNetDirty = true;
		}
	}
	else
	{
		// first ability modifier of this ability, create key and containing info map
		TTuple<TSubclassOf<UP4GameplayAbility>, FP4AbilityModifierInfoMapStruct> NewElem;
		NewElem.Key = AbilityClass;

		FP4AbilityModifierInfoMapStruct NewElemMap;
		NewElemMap.AbilityModToInfoMap.Add(TTuple<FGameplayTag, FP4AbilityModifierInfoStruct>(ModifierInfo.GrantedTag, ModifierInfo));
		NewElem.Value = NewElemMap;		
		AddLooseGameplayTag(ModifierInfo.GrantedTag);
		bIsNetDirty = true;
	}
}




void UP4PlayerAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(UP4PlayerAbilitySystemComponent, AbilityPools);
	DOREPLIFETIME(UP4PlayerAbilitySystemComponent, AbilityModifiers);
	DOREPLIFETIME(UP4PlayerAbilitySystemComponent, MaxAbilityPools);
}