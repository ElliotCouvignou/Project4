// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemBaseActor.h"
#include "Characters/P4PlayerCharacterBase.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AP4ItemBaseActor::AP4ItemBaseActor() : Super()
{
	

}

void AP4ItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

	// try to set item Static mesh based on data asset parameter
	SetItemStructAndStaticMesh(InventoryItemStruct);	
}

void AP4ItemBaseActor::GrantUniqueItemAbility()
{
}



void AP4ItemBaseActor::OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result)
{
	// TODO: find way to return result (bind delegate on successful add)
	if (SourceActor)
	{
		UP4InventoryBagComponent* IBC = SourceActor->GetInventoryBagComponent();
		if (IBC)
		{
			IBC->ServerAddItemToInventory(InventoryItemStruct, this);
		}
	}
}

void AP4ItemBaseActor::SetItemStructAndStaticMesh(const FInventoryItemStruct& InputInventoryItemStruct)
{
	InventoryItemStruct = InputInventoryItemStruct;

	// try to set item Static mesh based on data asset parameter
	if (InventoryItemStruct.ItemBaseDataAsset)
	{
		UStaticMesh* SM = InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemStaticMesh;
		if (SM != nullptr && StaticMesh)
		{
			StaticMesh->SetStaticMesh(SM);
		}
	}
}

// ovveride replciation with replication variables
void AP4ItemBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4ItemBaseActor, InventoryItemStruct);
}
