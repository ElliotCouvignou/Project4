// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AI/P4AreaToMobPoints.h"

UP4AreaToMobPoints::UP4AreaToMobPoints()
	: Super()
{

}

void UP4AreaToMobPoints::GetMobSpawnArrayFromLevelName(const FName& LevelName, FArrayOfMobSpawnInfo& Return)
{
	TArray<FName> Keys;
	LevelMapToMobs.GetKeys(Keys);

	for (FName name : Keys)
	{
		if (LevelName.ToString().Contains(name.ToString()))
		{
			Return = *LevelMapToMobs.Find(name);
			return;
		}
	}
	return;
}



void UP4AreaToMobPoints::GetRandomMob(FName CurrentLevelName, FMobSpawnInfo& Result)
{
	FArrayOfMobSpawnInfo Spawns;
	GetMobSpawnArrayFromLevelName(CurrentLevelName, Spawns);

	float TotalWeight = 0.f;
	if (Spawns.SpawnableMobs.Num() > 0)
	{
		if (Spawns.SpawnableMobs.Num() > 0)
		{
			TotalWeight = 0.f;
			for (auto Info : Spawns.SpawnableMobs)
			{
				TotalWeight += Info.Weight;
			}
		}

		float rand = FMath::RandRange(0.f, TotalWeight);
		for (auto Info : Spawns.SpawnableMobs)
		{
			rand -= Info.Weight;
			if (rand <= 0.f)
			{
				Result = Info;
				return;
			}
		}
	}
}

void UP4AreaToMobPoints::GetRandomBossMob(FName CurrentLevelName, FMobSpawnInfo& Result)
{
	FArrayOfMobSpawnInfo Spawns;
	GetMobSpawnArrayFromLevelName(CurrentLevelName, Spawns);

	float TotalWeight = 0.f;
	if (Spawns.SpawnableBosses.Num() > 0)
	{
		if (Spawns.SpawnableBosses.Num() > 0)
		{
			TotalWeight = 0.f;
			for (auto Info : Spawns.SpawnableBosses)
			{
				TotalWeight += Info.Weight;
			}
		}

		float rand = FMath::RandRange(0.f, TotalWeight);
		for (auto Info : Spawns.SpawnableBosses)
		{
			rand -= Info.Weight;
			if (rand <= 0.f)
			{
				Result = Info;
				return;
			}
		}
	}
}