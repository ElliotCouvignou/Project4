// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPointsActor.generated.h"

UCLASS()
class PROJECT4_API APatrolPointsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPointsActor();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetPatrolPointsInWorldSpace(TArray<FVector>& WorldPatrolPoints);

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetPatrolPointsInLocalSpace(TArray<FVector>& LocalPatrolPoints) { LocalPatrolPoints = PatrolPoints; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Patrol Points", Meta = (MakeEditWidget = true))
		TArray<FVector> PatrolPoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
