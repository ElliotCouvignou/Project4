// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4Interactable.h"
#include "NiagaraFunctionLibrary.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


// Sets default values
AP4Interactable::AP4Interactable()
{
 	//  turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(TEXT("Interactables"));

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
		print(FString("Activate Effect"));
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraEffect, RootComponent, FName(TEXT("None")), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), EAttachLocation::KeepRelativeOffset, true);
	}
}


void AP4Interactable::OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result)
{
	print(FString("OnInteract  bAse Called!"));

	Result = true;
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
