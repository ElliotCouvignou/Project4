// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "ProceduralDungeonGenerator.h"

// Sets default values
AProceduralDungeonGenerator::AProceduralDungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AProceduralDungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateDungeon();
}

void AProceduralDungeonGenerator::GenerateDungeon()
{
	// just testing spawns rn make straight line of 3 random connecting rooms

	Rooms.Empty();
	
	UWorld* world = GetWorld();
	if (!world)
		return;



	// Spawn Start room
	
}

void AProceduralDungeonGenerator::CreateStartingRoom()
{
}

// Called every frame
void AProceduralDungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProceduralDungeonGenerator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProject4Character, AttributeSet);
	DOREPLIFETIME(AProceduralDungeonGenerator, Seed);
}
