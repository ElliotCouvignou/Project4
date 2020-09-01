// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemBaseActor.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "UI/ItemActorNameWidget.cpp"



#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AP4ItemBaseActor::AP4ItemBaseActor() : Super()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(StaticMesh);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);

	if (ItemNameWidgetClass)
	{
		WidgetComponent->SetWidget(Cast<UUserWidget>(ItemNameWidgetClass));
	}
	
	
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

		MultiCastWidgetTextName(InputInventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName, InputInventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemRank);
	}

	// Relays inventoryItemStruct to others
	
}

void AP4ItemBaseActor::MultiCastWidgetTextName_Implementation(const FString& Name, EItemRank ItemRank)
{
	UItemActorNameWidget* NameWidget = Cast<UItemActorNameWidget>(WidgetComponent->GetUserWidgetObject());
	if (NameWidget)
	{
		NameWidget->SetItemName(Name, ItemRank);
	}
}

// ovveride replciation with replication variables
void AP4ItemBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4ItemBaseActor, InventoryItemStruct);
}
