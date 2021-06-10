// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "UObject/NoExportTypes.h"
#include "Asset/PrefabricatorAsset.h"

#include "ProceduralDungeonRoom.generated.h"

class UPrefabricatorConstructionSnapComponent;
class APrefabActor;


UENUM(BlueprintType)
enum class ERoomDirections : uint8
{
	PosX			UMETA(DisplayName = "PosX"),
	NegX			UMETA(DisplayName = "NegX"),
	PosY			UMETA(DisplayName = "PosY"),
	NegY			UMETA(DisplayName = "NegY")
};

/**
 * 
 */
UCLASS()
class PROCEDURALDUNGEONS_API UProceduralDungeonRoom : public UObject
{
	GENERATED_BODY()


public:



	UFUNCTION()
		void SpawnFromPrefabInterface(UPrefabricatorAssetInterface* Prefab, FTransform Transform, float Seed);

	UFUNCTION()
		void CreateNeighborRoom(ERoomDirections Direction, UPrefabricatorAssetInterface* Prefab, float Seed);

	

	/* returns FTransform so that objects are snapped to each other*/
	//UFUNCTION()
	//	FTransform FindConnectingPoint(UPrefabricatorAssetInterface* Room, UPrefabricatorAssetInterface* ConnectingRoom);

	ERoomDirections GetOppositeRoomDirection(ERoomDirections Direction);


	TSoftObjectPtr<UPrefabricatorAssetInterface*> RoomPrefabInterface;
	
	APrefabActor* SpawnedPrefab;

	/* Conencting Dungeon Rooms to this one */
	TMap<ERoomDirections, UProceduralDungeonRoom*> Neighbors;

	UPROPERTY(Transient)
		TMap<ERoomDirections, UPrefabricatorConstructionSnapComponent*> SnapComponents;
};
