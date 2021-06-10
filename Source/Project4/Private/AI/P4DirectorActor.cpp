// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AI/P4DirectorActor.h"
#include "AI/P4AreaToMobPoints.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

// Sets default values
AP4DirectorActor::AP4DirectorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
	//PointTimerRate_s = 1.f;
	//PointTimerAmount = 1.f;

	
}


void AP4DirectorActor::AddPointsToMax(float AmountToAdd)
{
	CurrentPoints += AmountToAdd;
	QueryMobSpawn();
}

void AP4DirectorActor::QueryMobSpawn()
{
	// determines whether or not to spawn mobs and waht kinds, actual mob spawning handled separately
	if (SpawnableMobs.Num() > 0)
	{
		// TODO: check for Overcrowding here (find # of spawned mobs in current map)
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AP4MobCharacterBase::StaticClass(), FoundActors);
		int numSpawned = FoundActors.Num();
		
		if (numSpawned >= MaximumSpawnCount)
		{
			return;
		}
			
		// Create wave to spawn, select 
		TArray<FMobSpawnParameters> MobstoSpawn;
		float Cost = 0.f;

		int WaveSize = (int)FMath::RandRange(float(MinimumSpawnCount),FMath::Min((float)(MaximumSpawnCount - numSpawned), float(MaximumSpawnCount) + 0.99f));
		while (MobstoSpawn.Num() < WaveSize)
		{
			FMobSpawnInfo MobInfo;
			
			GetRandomMob(MobInfo);
			if (MobInfo.MobClass)
			{
				// TODO: determine if this mob is too cheap and can be upgraded to higher elite status 

				FMobSpawnParameters NewMob;
				NewMob.MobClass = MobInfo.MobClass;

				MobstoSpawn.Add(NewMob);
				Cost = MobInfo.PointCost;
			}
		}

		if (Cost > CurrentPoints)
		{
			return; // not in budget, just give up and wait for next query
		}

		SpawnMobWave(MobstoSpawn);
		CurrentPoints -= Cost;
	}
}



void AP4DirectorActor::SpawnMobWave(TArray<FMobSpawnParameters> MobSpawns)
{
	// hopefully this wont change since we can abstract the call to find locations to spawn
	/* we want spawns to be in pack so find a spawn location and then */
	UWorld* World = GetWorld();

	FVector SpawnLoc = GetRandomSpawnLocation();
	for (auto Mob : MobSpawns)
	{
		

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		World->SpawnActor<AProject4Character>(Mob.MobClass, SpawnLoc, FRotator(0.f, FMath::FRand(), 0.f), SpawnInfo);

		print(FString("Spawned Mob: Location: " + SpawnLoc.ToString()));
	}
}

FVector AP4DirectorActor::GetRandomSpawnLocation()
{
	// find location around self and spawn TODO: determine to use this andor other part after.
	FNavLocation Location;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys && NavSys->GetRandomReachablePointInRadius(GetActorLocation(), SpawnRadius, Location))
	{
		return Location.Location;
	}

	// Get Players to spawn on 
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AP4PlayerCharacterBase::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		// Choose random person to spawn on 
		print(FString("Spawn on player"));
		AActor* ActorToSpawnOn = FoundActors[(int)FMath::RandRange(0.f, (float)FoundActors.Num() - 0.00001f)];
		return FVector(FMath::RandRange(-SpawnRadius, SpawnRadius), FMath::RandRange(-SpawnRadius, SpawnRadius), 10.f) + ActorToSpawnOn->GetActorLocation();
	}
	
	return FVector(0.f, 0.f, 0.f);
}


void AP4DirectorActor::SetSpawnableMobsArray(UP4AreaToMobPoints* DataAsset)
{
	if (GetWorld() && DataAsset)
	{
		FName CurrentLevelName = FName(GetWorld()->GetMapName());

		FArrayOfMobSpawnInfo SpawnArray;
		DataAsset->GetMobSpawnArrayFromLevelName(CurrentLevelName, SpawnArray);
		if (SpawnArray.SpawnableMobs.Num() > 0)
		{
			SpawnableMobs = SpawnArray.SpawnableMobs;
			UE_LOG(LogTemp, Warning, TEXT("P4Director found SpawnableMobsArray"));

			// Set some info that we would rather not figure out many times over 
			if (SpawnableMobs.Num() > 0)
			{
				TotalWeight = 0.f;
				for (auto Info : SpawnableMobs)
				{
					TotalWeight += Info.Weight;
				}
			}
		}
		
	}	
}

void AP4DirectorActor::OnPointTimerUpdate()
{
	AddPointsToMax(PointTimerAmount);
}

// Called when the game starts or when spawned
void AP4DirectorActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (DoTimerMethod)
		StartPointTimer();
	else
		QueryMobSpawn();
}

void AP4DirectorActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void AP4DirectorActor::StartPointTimer()
{
	GetWorld()->GetTimerManager().SetTimer(PointTimerHandle, this, &AP4DirectorActor::OnPointTimerUpdate, PointTimerRate_s, true);
}


void AP4DirectorActor::GetRandomMob(FMobSpawnInfo& RandMob)
{
	if (SpawnableMobs.Num() > 0)
	{
		float rand = FMath::RandRange(0.f, TotalWeight);
		for (auto Info : SpawnableMobs)
		{
			rand -= Info.Weight;
			if (rand <= 0.f)
			{
				RandMob = Info;
				return;
			}
		}
	}
}


// Called every frame
void AP4DirectorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

