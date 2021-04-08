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
