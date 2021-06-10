// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4Interactable.h"
#include "NiagaraFunctionLibrary.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


// Sets default values
AP4Interactable::AP4Interactable() : Super()
{
	// Enable replication and relevancy
	bReplicates = true;
	bAlwaysRelevant = true;

 	//  turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(TEXT("Interactables"));
	
	//RootComponent->SetIsReplicated(true);
	StaticMesh->SetIsReplicated(true);

	//NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	//NiagaraComponent->SetupAttachment(RootComponent);
	//
	//if (NiagaraEffect)
	//{
	//	NiagaraComponent->SetAsset(NiagaraEffect);
	//}
}

// Called when the game starts or when spawned
void AP4Interactable::BeginPlay()
{
	Super::BeginPlay();

	if (bVFXOnConstruct)
	{
		StartNiagaraEffect();
	}
	
}

void AP4Interactable::StartNiagaraEffect()
{
	if (/*NiagaraComponent &&*/ NiagaraEffect)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraEffect, RootComponent, FName(TEXT("None")), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), EAttachLocation::KeepRelativeOffset, true);
	}
}


void AP4Interactable::ServerOnInteract_Implementation(const AP4PlayerCharacterBase* SourceActor)
{
	BP_OnInteract(SourceActor);
	OnInteract(SourceActor);	
}

void AP4Interactable::OnInteract(const AP4PlayerCharacterBase* SourceActor)
{
	print(FString("OnInteract  bAse Called!"));

}

void AP4Interactable::BP_OnInteract_Implementation(const AP4PlayerCharacterBase* SourceActor)
{

}

void AP4Interactable::BeginDestroy()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}

	Super::BeginDestroy();
}

// Called every frame
void AP4Interactable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

