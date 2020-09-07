// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/PatrolPointsActor.h"

// Sets default values
APatrolPointsActor::APatrolPointsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

void APatrolPointsActor::GetPatrolPointsInWorldSpace(TArray<FVector>& WorldPatrolPoints)
{
	WorldPatrolPoints = PatrolPoints;
	FVector WorldLocation = GetActorLocation();

	for (FVector& vec : WorldPatrolPoints)
	{
		vec += WorldLocation;
	}	
}

// Called when the game starts or when spawned
void APatrolPointsActor::BeginPlay()
{
	Super::BeginPlay();
	
}


