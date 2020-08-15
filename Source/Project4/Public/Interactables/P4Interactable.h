// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P4Interactable.generated.h"

UCLASS()
class PROJECT4_API AP4Interactable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP4Interactable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
