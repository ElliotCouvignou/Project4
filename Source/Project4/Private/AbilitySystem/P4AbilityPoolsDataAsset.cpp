// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4AbilityPoolsDataAsset.h"

void UP4AbilityPoolsDataAsset::GetAbilityCategoriesFromPools(TArray<EClassAbilityPoolType>& AbilityPools, TArray<EAbilityCategory>& Result)
{
	Result.Empty();
	for (EClassAbilityPoolType type : AbilityPools)
	{
		if (PoolToInfoMap.Contains(type))
		{
			TArray<EAbilityCategory> Categories;
			PoolToInfoMap.Find(type)->CategoryToInfoMap.GetKeys(Categories);

			for (EAbilityCategory cat : Categories)
				Result.Add(cat);
		}
	}
}

void UP4AbilityPoolsDataAsset::GetAbilitiesFromPoolsAndCategory(TArray<EClassAbilityPoolType>& AbilityPools, EAbilityCategory Category, TArray<TSubclassOf<UP4GameplayAbility>>& Result)
{
	Result.Empty();
	for (EClassAbilityPoolType type : AbilityPools)
	{
		if (PoolToInfoMap.Contains(type) && PoolToInfoMap.Find(type)->CategoryToInfoMap.Contains(Category))
		{
			TArray<FP4AbilityPoolAbilityInfoStruct>& Infos= PoolToInfoMap.Find(type)->CategoryToInfoMap.Find(Category)->AbilityInfos;
			for(FP4AbilityPoolAbilityInfoStruct& info : Infos)
				Result.Add(info.Ability);
		}
	}
}

void UP4AbilityPoolsDataAsset::GetAbilitiesFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<TSubclassOf<UP4GameplayAbility>>& Result)
{
	if (AbilityPools.Num() <= 0)
		return;

	Result.Empty();
	for (EClassAbilityPoolType type : AbilityPools)
	{
		if (PoolToInfoMap.Contains(type))
		{
			TArray<FP4AbilityPoolAbilityInfoStruct> Infos;
			TArray<FP4AbilityPoolAbilityArrayStruct> Arrays;
			PoolToInfoMap.Find(type)->CategoryToInfoMap.GenerateValueArray(Arrays);
			for (FP4AbilityPoolAbilityArrayStruct& arr : Arrays)
			{
				Infos.Append(arr.AbilityInfos);
			}

			for (FP4AbilityPoolAbilityInfoStruct& info : Infos)
				Result.Add(info.Ability);
		}
	}
}

void UP4AbilityPoolsDataAsset::GetAbilityInfoStructFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<FP4AbilityPoolAbilityInfoStruct>& Result)
{
	if (AbilityPools.Num() <= 0)
		return;

	Result.Empty();
	for (EClassAbilityPoolType type : AbilityPools)
	{
		if (PoolToInfoMap.Contains(type))
		{
			TArray<FP4AbilityPoolAbilityArrayStruct> Arrays;
			PoolToInfoMap.Find(type)->CategoryToInfoMap.GenerateValueArray(Arrays);
			for (FP4AbilityPoolAbilityArrayStruct& arr : Arrays)
			{
				Result.Append(arr.AbilityInfos);
			}
		}
	}
}

void UP4AbilityPoolsDataAsset::GetAbilityInfoStructOutsidePools(const TArray<EClassAbilityPoolType>& AbilityPools, TArray<FP4AbilityPoolAbilityInfoStruct>& Result)
{
	if (AbilityPools.Num() <= 0)
		return;

	Result.Empty();
	TArray< EClassAbilityPoolType> Types;
	PoolToInfoMap.GetKeys(Types);
	for (EClassAbilityPoolType type : Types)
	{
		if (!AbilityPools.Contains(type))
		{
			TArray<FP4AbilityPoolAbilityArrayStruct> Arrays;
			PoolToInfoMap.Find(type)->CategoryToInfoMap.GenerateValueArray(Arrays);
			for (FP4AbilityPoolAbilityArrayStruct& arr : Arrays)
			{
				Result.Append(arr.AbilityInfos);
			}
		}
	}
}

void UP4AbilityPoolsDataAsset::GetLearnedAbilityInfoFromPools(const TArray<EClassAbilityPoolType>& AbilityPools, const TArray<TSubclassOf<UP4GameplayAbility>>& LearnedAbilities, TArray<FP4AbilityPoolAbilityInfoStruct>& Result)
{
	if (AbilityPools.Num() <= 0)
		return;

	Result.Empty();
	for (EClassAbilityPoolType type : AbilityPools)
	{
		if (PoolToInfoMap.Contains(type))
		{
			TArray<FP4AbilityPoolAbilityInfoStruct> Infos;
			TArray<FP4AbilityPoolAbilityArrayStruct> Arrays;
			PoolToInfoMap.Find(type)->CategoryToInfoMap.GenerateValueArray(Arrays);
			for (FP4AbilityPoolAbilityArrayStruct& arr : Arrays)
			{
				Infos.Append(arr.AbilityInfos);
			}

			for (FP4AbilityPoolAbilityInfoStruct& info : Infos)
			{
				if (LearnedAbilities.Contains(info.Ability))
					Result.Add(info);
			}
				
		}
	}
}

void UP4AbilityPoolsDataAsset::GetAbilityInfoStructs(const TArray<EClassAbilityPoolType>& AbilityPools, const TSubclassOf<UP4GameplayAbility> AbilityClass, TArray<FP4AbilityPoolAbilityInfoStruct>& Result)
{
	if (AbilityPools.Num() <= 0)
		return;

	Result.Empty();
	for (EClassAbilityPoolType type : AbilityPools)
	{
		if (PoolToInfoMap.Contains(type))
		{
			TArray<FP4AbilityPoolAbilityInfoStruct> Infos;
			TArray<FP4AbilityPoolAbilityArrayStruct> Arrays;
			PoolToInfoMap.Find(type)->CategoryToInfoMap.GenerateValueArray(Arrays);
			for (FP4AbilityPoolAbilityArrayStruct& arr : Arrays)
			{
				Infos.Append(arr.AbilityInfos);
			}

			for (FP4AbilityPoolAbilityInfoStruct& info : Infos)
			{
				if (AbilityClass == info.Ability)
					Result.Add(info);
			}

		}
	}
}
