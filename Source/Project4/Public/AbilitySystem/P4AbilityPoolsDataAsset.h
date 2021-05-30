// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Project4.h"
#include "AbilitySystem/P4AbilityModifier.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "P4AbilityPoolsDataAsset.generated.h"




USTRUCT(BlueprintType)
struct FP4AbilityPoolAbilityInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/* Tag granted to player to gain this modifier (stackeable if allowed) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<UP4GameplayAbility> Ability;

	FP4AbilityPoolAbilityInfoStruct()
	{

	}
	FP4AbilityPoolAbilityInfoStruct(const FP4AbilityPoolAbilityInfoStruct& other)
	{
		Ability = other.Ability;
	}

	bool operator==(const FP4AbilityPoolAbilityInfoStruct& B)
	{
		// TODO: maybe make this comapre all elements in abilitymodifiers if two pools share same spell with diff modifiers
		return this->Ability == B.Ability;
	}
};

USTRUCT(BlueprintType)
struct FP4AbilityPoolAbilityArrayStruct
{
	GENERATED_USTRUCT_BODY()

	/* Tag granted to player to gain this modifier (stackeable if allowed) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
		TArray<FP4AbilityPoolAbilityInfoStruct> AbilityInfos;
};

USTRUCT(BlueprintType)
struct FP4AbilityPoolCategoryMapStruct
{
	GENERATED_USTRUCT_BODY()

		/* Tag granted to player to gain this modifier (stackeable if allowed) */
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essentials")
		TMap<EAbilityCategory, FP4AbilityPoolAbilityArrayStruct> CategoryToInfoMap;
};


UENUM(BlueprintType)
enum class EP4AbilityNodeType : uint8
{
	Pool			UMETA(DisplayName = "Pool"),
	Category		UMETA(DisplayName = "Category"),
	Ability			UMETA(DisplayName = "Ability")
};

/**
 * Replacement for ability pool genericgraphs since they suck and keep deleting data on project launch
 */
UCLASS()
class PROJECT4_API UP4AbilityPoolsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public: 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Essentials")
		TMap<EClassAbilityPoolType, FP4AbilityPoolCategoryMapStruct> PoolToInfoMap;

	/* Gets all the category pools that the given abilitypools have */
	UFUNCTION(BlueprintCallable)
		void GetAbilityCategoriesFromPools(TArray<EClassAbilityPoolType>& AbilityPools, TArray<EAbilityCategory>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilitiesFromPoolsAndCategory(TArray<EClassAbilityPoolType>& AbilityPools, EAbilityCategory Category, TArray<TSubclassOf<UP4GameplayAbility>>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilitiesFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<TSubclassOf<UP4GameplayAbility>>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilityInfoStructFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<FP4AbilityPoolAbilityInfoStruct>& Result);

	/* Gathers info structs excluded from pools */
	UFUNCTION(BlueprintCallable)
		void GetAbilityInfoStructOutsidePools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<FP4AbilityPoolAbilityInfoStruct>& Result);

	UFUNCTION(BlueprintCallable)
		void GetLearnedAbilityInfoFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, const TArray<TSubclassOf<UP4GameplayAbility>>& LearnedAbilities, TArray<FP4AbilityPoolAbilityInfoStruct>& Result);

	UFUNCTION(BlueprintCallable)
		void GetAbilityInfoStructs(const TArray<EClassAbilityPoolType>& AbilityPools, const TSubclassOf<UP4GameplayAbility> AbilityClass, TArray<FP4AbilityPoolAbilityInfoStruct>& Result);
};
