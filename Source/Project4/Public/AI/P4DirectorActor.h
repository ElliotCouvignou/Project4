// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/P4MobCharacterBase.h"
#include "AI/P4AreaToMobPoints.h"
#include "P4DirectorActor.generated.h"

USTRUCT(BlueprintType)
struct FMobSpawnParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essentials")
		TSubclassOf<AP4MobCharacterBase> MobClass;

	/* More here later or elsewhere to send info that this mob is elite and/or contains extra buffs for spice */
	

	FMobSpawnParameters()
	{

	}

};
/* This actor is responsible for being the director to spawn based on MaxPoints
*  Maxpoints will change based on time, difficulty, # current spawns, etc.
*/
UCLASS()
class PROJECT4_API AP4DirectorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP4DirectorActor();

	UFUNCTION(BlueprintCallable)
		void AddPointsToMax(float AmountToAdd);

	UFUNCTION(BlueprintCallable)
		void QueryMobSpawn();

	UFUNCTION(BlueprintCallable)
		void SetSpawnableMobsArray(UP4AreaToMobPoints* DataAsset);

	UFUNCTION(BlueprintCallable)
		void OnPointTimerUpdate();

	UFUNCTION(BlueprintCallable)
		void StartPointTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* to stop spawning one thing we can only spawn groups of at least 4 mos */
	UPROPERTY()
		int MinimumSpawnCount = 1;

	UPROPERTY()
		int MaximumSpawnCount = 5;

	/* At what percent of current/max points do we try to spawn */
	UPROPERTY()
		float PercentToTrySpawn = 0.8f;

	/* what % of max budget will we roll for on lows*/
	UPROPERTY()
		float MinBudgetPercent = 0.4f;

	UPROPERTY()
		float MaxPoints;
	
	UPROPERTY()
		float CurrentPoints;

	/* Number of mobs spawned by this director */
	UPROPERTY()
		int SpawnCount = 0;

	UPROPERTY()
		TArray<FMobSpawnInfo> SpawnableMobs;

	/* # of weights summed in spawnableMobs array */
	UPROPERTY()
		float TotalWeight;

	/* rate of timer that increments Points*/
	UPROPERTY()
		float PointTimerRate_s = 1.f;

	/* Amount of points given whenever point timer updates */
	UPROPERTY()
		float PointTimerAmount = 1.f;

	UPROPERTY()
		FTimerHandle PointTimerHandle;




	UFUNCTION(BlueprintCallable)
		void GetRandomMob(FMobSpawnInfo& RandMob);

	UFUNCTION(BlueprintCallable)
		void SpawnMobWave(TArray<FMobSpawnParameters> MobSpawns);

	UFUNCTION(BlueprintCallable)
		FVector GetRandomSpawnLocation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents();

};
