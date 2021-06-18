// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Interactables/P4Interactable.h"
#include "TeleporterActor.generated.h"

/**
 * 
 */

class AProject4Character;

UCLASS()
class PROJECT4_API ATeleporterActor : public AP4Interactable
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ATeleporterActor();

	virtual void OnInteract(const AP4PlayerCharacterBase* SourceActor) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsEnabled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AProject4Character* Boss;

	UFUNCTION(BlueprintCallable)
		void SpawnBoss();

	UFUNCTION()
		void OnBossDied(AProject4Character* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
