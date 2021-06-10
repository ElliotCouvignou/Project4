// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Templates/SubclassOf.h"
#include "Project4.h"
#include "GameplayAbilitySpec.h"
#include "P4GameplayAbilitySet.generated.h"


class UAbilitySystemComponent;

/**
 *	Example struct that pairs a enum input command to a GameplayAbilityClass.6
 */
USTRUCT(BlueprintType)
struct FP4GameplayAbilityBindInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BindInfo)
		EP4AbilityInputID	Command;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BindInfo)
		TSubclassOf<class UP4GameplayAbility>	AbilityClass;

	//bool operator==(const class UGameplayAbility& lhs) const;
};


/**
 * 
 */
UCLASS()
class PROJECT4_API UP4GameplayAbilitySet : public UDataAsset
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, Category = AbilitySet)
		TArray<FP4GameplayAbilityBindInfo>	Abilities;

	/* Array to set default hotbar bindings on these abilities (e.g beserker starts off with lmb and rmb bound to abilities) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = AbilitySet)
		TArray<FP4GameplayAbilityBindInfo>	DefaultBoundAbilities;

	void GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const;
		

	void AddClassesToRemove(TArray<FGameplayAbilitySpecHandle>& RemoveArray, TArray<FGameplayAbilitySpec>& AbilitiesArray);
};
