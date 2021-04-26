// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GenericGraph.h"
#include "GenericGraph/Abilities/P4AbilityNode.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h" // only necessary for EAbilityPoolType
#include "P4AbilityPoolGraph.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT4_API UP4AbilityPoolGraph : public UGenericGraph
{
	GENERATED_BODY()

public:
	UP4AbilityPoolGraph();

	/* Gets all the category pools that the given abilitypools have */
	UFUNCTION(BlueprintCallable)
		void GetAbilityCategoriesFromPools(TArray<EClassAbilityPoolType>& AbilityPools, TArray<EAbilityCategory>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilitiesFromPoolsAndCategory(TArray<EClassAbilityPoolType>& AbilityPools, EAbilityCategory Category, TArray<TSubclassOf<UP4GameplayAbility>>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilitiesFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<TSubclassOf<UP4GameplayAbility>>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilityNodesFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<UP4AbilityNode*>& Result);

	UFUNCTION(BlueprintCallable)
		void GetLearnedAbilityNodesFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, const TArray<TSubclassOf<UP4GameplayAbility>>& LearnedAbilities, TArray<UP4AbilityNode*>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilityNodes(const TArray<EClassAbilityPoolType>& AbilityPools, const TSubclassOf<UP4GameplayAbility> AbilityClass, TArray<UP4AbilityNode*>& Result);

	UPROPERTY(EditDefaultsOnly, Category = "Ability Pool")
		FLinearColor AbilityPoolColor;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Pool")
		TArray<UP4AbilityPoolGraph*> SubPools; // Unused for now but setup for later maybe idk 
	
};
