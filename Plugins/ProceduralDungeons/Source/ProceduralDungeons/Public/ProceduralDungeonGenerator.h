// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "ProceduralDungeonRoom.h"
#include "Asset/PrefabricatorAsset.h"
#include "Utils/ConstructionSystemUtils.h"

#include "ProceduralDungeonGenerator.generated.h"




/* This is the base class and meant to be subclassed for different generation methods */
UCLASS()
class PROCEDURALDUNGEONS_API AProceduralDungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralDungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		virtual void GenerateDungeon();

	UFUNCTION()
		virtual void CreateStartingRoom();

	// first index will be starting and TODO: last will be final boss
	UPROPERTY()
		TArray<UProceduralDungeonRoom*> Rooms;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "Prefabs")
		UPrefabricatorAssetInterface* StartingLevelPrefab;

	UPROPERTY(EditAnywhere, Category = "Prefabs")
		TArray<UPrefabricatorAssetInterface*> RegularRoomPrefabs;

	UPROPERTY(EditAnywhere, Category = "Prefabs")
		TArray<UPrefabricatorAssetInterface*> BossRoomPrefabs;

	/* Replicated Seed value for generating dungeons MAKE SURE EVERYTHING USES THIS FOR MULTIPLAYER REPLICATION */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
		float Seed;
};
