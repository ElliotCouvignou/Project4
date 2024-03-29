// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/P4Interactable.h"
#include "SkillModifierDropActor.generated.h"



UCLASS()
class PROJECT4_API ASkillModifierDropActor : public AP4Interactable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillModifierDropActor();


	/* if true, thios actor doesnt get destroyed after interaction */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool bIsTestActor = false;

	virtual void OnInteract(const AP4PlayerCharacterBase* SourceActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		TArray<APlayerController*> SeenPlayers;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
