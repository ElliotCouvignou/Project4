// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemBaseActor.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/ItemActorNameWidget.cpp"



#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,text)


AP4ItemBaseActor::AP4ItemBaseActor() : Super()
{
	static ConstructorHelpers::FClassFinder<UItemActorNameWidget> NameWidgetClass(TEXT("/Game/Project4/UI/Gameplay/GeneratedUI/BP_ItemActorName"));
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
	UStaticMesh* SM = InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemStaticMesh;
	if (SM != nullptr && StaticMesh)
	{
		StaticMesh->SetStaticMesh(SM);
	}

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
		NameWidget->SetItemName(InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemName, InventoryItemStruct.ItemBaseDataAsset->ItemInfo.ItemRank);
	}
}

void AP4ItemBaseActor::GrantUniqueItemAbility()
{
	// TODO: this
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


// ovveride replciation with replication variables
void AP4ItemBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4ItemBaseActor, InventoryItemStruct);
}
