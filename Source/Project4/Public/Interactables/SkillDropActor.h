// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/P4Interactable.h"
#include "SkillDropActor.generated.h"





UCLASS()
class PROJECT4_API ASkillDropActor : public AP4Interactable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillDropActor();


	virtual void OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
