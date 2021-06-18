// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Characters/P4MobCharacterBase.h"
#include "P4AreaToMobPoints.generated.h"


USTRUCT(BlueprintType)
struct FMobSpawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TSubclassOf<AP4MobCharacterBase> MobClass;

	/* # of points needed to spawn one of this mob*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float PointCost;

	/* weight for spawner to roll for this mob, higher = more chance */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		float Weight;

	FMobSpawnInfo()
	{
		MobClass = nullptr;
		PointCost = 1.f;
		Weight = 1.f;
	}

};

USTRUCT(BlueprintType)
struct FArrayOfMobSpawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FMobSpawnInfo> SpawnableMobs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TArray<FMobSpawnInfo> SpawnableBosses;

	FArrayOfMobSpawnInfo()
	{

	}

};


/**
 * 
 */
UCLASS(BlueprintType)
class PROJECT4_API UP4AreaToMobPoints : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UP4AreaToMobPoints();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TMap<FName, FArrayOfMobSpawnInfo> LevelMapToMobs;

	UFUNCTION(BlueprintCallable)
		void GetMobSpawnArrayFromLevelName(const FName& LevelName, FArrayOfMobSpawnInfo& Return);


	UFUNCTION(BlueprintCallable)
		void GetRandomMob(FName CurrentLevelName, FMobSpawnInfo& Result);

	UFUNCTION(BlueprintCallable)
		void GetRandomBossMob(FName CurrentLevelName, FMobSpawnInfo& Result);
};
