// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4Interactable.h"

// Sets default values
AP4Interactable::AP4Interactable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AP4Interactable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP4Interactable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

 