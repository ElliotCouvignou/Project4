// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/P4AbilityModifier.h"
#include "AbilitySystem/P4AbilityPoolsDataAsset.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h" // only necessary for EAbilityPoolType
#include "P4AbilityNode.generated.h"




class UP4GameplayAbility;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECT4_API UP4AbilityNode : public UGenericGraphNode
{
	GENERATED_BODY()

public:
	UP4AbilityNode();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		EP4AbilityNodeType NodeType = EP4AbilityNodeType::Ability;

	/* Only valid when nodetype is ability */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<UP4GameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pool")
		EClassAbilityPoolType PoolType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Category")
		EAbilityCategory CategoryType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TArray<TSubclassOf<UP4AbilityModifierInfo>> AbilityModifiers;

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetLeafNodes(TArray<UP4AbilityNode*>& Result);

#if WITH_EDITOR 

	virtual FLinearColor GetBackgroundColor() const override;
#endif

private:

	UFUNCTION()
		void GetLeafNodesRecursive(TArray<UP4AbilityNode*>& Result);


};



