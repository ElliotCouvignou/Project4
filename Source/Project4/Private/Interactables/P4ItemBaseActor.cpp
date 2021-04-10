// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemBaseActor.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/ItemActorNameWidget.cpp"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" 


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AP4ItemBaseActor::AP4ItemBaseActor() : Super()
{
	static ConstructorHelpers::FClassFinder<UItemActorNameWidget> NameWidgetClass(TEXT("/Game/Project4/UI/Gameplay/MainUI/GeneratedUI/BP_ItemActorName"));
	if (NameWidgetClass.Class)
	{
		ItemNameWidgetClass = NameWidgetClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ConstructorHelpers::FClassFinder Failed to find ItemActorName Widget!!"));
	}

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(StaticMesh);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	
	auto Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/AdvancedLocomotionV4/Props/Meshes/Barrel.Barrel'"));
	if (Mesh.Object)
	{
		StaticMesh->SetStaticMesh(Mesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Defaulted staticmesh asset for AP4ItemBaseActor()"));
	}

	if (ItemNameWidgetClass)
	{
		WidgetComponent->SetWidgetClass(ItemNameWidgetClass);
		WidgetComponent->UpdateWidget();
	}
}



void AP4ItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

	// try to set item Static mesh based on data asset parameter
	//SetItemStructAndStaticMesh(InventoryItemStruct);	
	UStaticMesh* SM = (InventoryItemStruct.ItemBaseDataAsset) ? InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemStaticMesh : InventoryItemStruct.ItemInfoStruct.ItemStaticMesh;
	if (SM)
	{
		StaticMesh->SetStaticMesh(SM);
	}

	// HACK: set all visibility to false, let render dist sphere collision set vis to true when needed
	RootComponent->SetVisibility(false, true);

	MoveToGround();
	SetWidgetName();
}

void AP4ItemBaseActor::MoveToGround()
{
	FVector End = GetActorLocation() + FVector(0.f, 0.f, -1.f) * 1500.f;
	TArray<AActor*> Ignore;
	Ignore.Add(this);
	FHitResult HitResult;
	if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), GetActorLocation(), End, FName("ItemGroundTrace"), false, Ignore, EDrawDebugTrace::None, HitResult, true))
	{
		SetActorLocation(HitResult.ImpactPoint);
	}
}

void AP4ItemBaseActor::SetWidgetName()
{
	// Set Widget Name
	UItemActorNameWidget* NameWidget = Cast<UItemActorNameWidget>(WidgetComponent->GetUserWidgetObject());
	if (NameWidget)
	{
		// true if this wasn't randomly generated (itembasedata asset ref is left empty)
		if (InventoryItemStruct.ItemBaseDataAsset)
		{
			NameWidget->SetItemName(InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName, InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemRank);
		}
		else
		{
			NameWidget->SetItemName(InventoryItemStruct.ItemInfoStruct.ItemName, InventoryItemStruct.ItemInfoStruct.ItemRank);
		}
		
	}
}

void AP4ItemBaseActor::GrantUniqueItemAbility()
{
	// TODO: this
}



void AP4ItemBaseActor::OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result)
{
	// TODO: find way to return result (bind delegate on successful add)
	print(FString("OnInteract"));
	if (SourceActor)
	{
		UP4InventoryBagComponent* IBC = SourceActor->GetInventoryBagComponent();
		if (IBC)
		{
			print(FString("Addtoinv"));
			IBC->ServerAddItemToInventory(InventoryItemStruct, this);
		}
	}
}


// ovveride replciation with replication variables
void AP4ItemBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4ItemBaseActor, InventoryItemStruct);
}
