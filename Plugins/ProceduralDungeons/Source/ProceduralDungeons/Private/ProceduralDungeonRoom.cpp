// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "ProceduralDungeonRoom.h"
#include "Asset/PrefabricatorAsset.h"
#include "ConstructionSystem/ConstructionSystemSnap.h"
#include "Utils/ConstructionSystemUtils.h"
#include "Prefab/PrefabTools.h"



void UProceduralDungeonRoom::SpawnFromPrefabInterface(UPrefabricatorAssetInterface* Prefab, FTransform Transform, float Seed)
{
	if (Prefab)
	{
		SpawnedPrefab = FConstructionSystemUtils::ConstructPrefabItem(GetWorld(), Prefab, Transform, Seed);
		
		RoomPrefabInterface = Prefab;

		// find snap components within prefab and save them for ref later
		FPrefabTools::IterateChildrenRecursive(SpawnedPrefab, [this](AActor* ChildActor) {
			if (ChildActor) {
				if (ChildActor->GetRootComponent()) {
					ChildActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
				}

				ChildActor->SetActorEnableCollision(false);

				for (UActorComponent* Component : ChildActor->GetComponents()) {
					if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component)) {
						if (!Primitive->IsA<UPrefabricatorConstructionSnapComponent>()) {
							// Disable collision
							Primitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}

					if (UPrefabricatorConstructionSnapComponent* SnapComponent = Cast<UPrefabricatorConstructionSnapComponent>(Component)) {
						
					//	SnapComponents.Add(SnapComponent);
					}
				}
			}
		});
	}
}

void UProceduralDungeonRoom::CreateNeighborRoom(ERoomDirections Direction, UPrefabricatorAssetInterface* Prefab, float Seed)
{
	if (Prefab && !Neighbors.Contains(Direction))
	{
		UProceduralDungeonRoom* NewRoom = NewObject<UProceduralDungeonRoom>();
		NewRoom->SpawnFromPrefabInterface(Prefab, /* TODO: find transform to connect */ FTransform(), Seed);

		// Create door and connect
		//UPrefabricatorConstructionSnapComponent* SnapHost = 
		//
		//if (FConstructionSystemUtils::GetSnapPoint(SnapHost, CursorSnap, Hit.ImpactPoint, TargetSnapTransform, CursorRotationStep, 100)) {
		//	bCursorModeFreeForm = false;
		//	CursorLocation = TargetSnapTransform.GetLocation();
		//	CursorRotation = TargetSnapTransform.GetRotation();
		//	DrawDebugPoint(World, CursorLocation, 20, FColor::Blue);
		//}
		
	}
}




ERoomDirections UProceduralDungeonRoom::GetOppositeRoomDirection(ERoomDirections Direction)
{
	switch (Direction)
	{
	case(ERoomDirections::NegX):
		return ERoomDirections::PosX;
		break;
	case(ERoomDirections::PosX):
		return ERoomDirections::NegX;
		break;
	case(ERoomDirections::NegY):
		return ERoomDirections::PosY;
		break;
	case(ERoomDirections::PosY):
		return ERoomDirections::NegY;
	}

	return ERoomDirections::NegX;
}