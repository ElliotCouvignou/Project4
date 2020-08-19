// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "P4Interactable.generated.h"

class AP4PlayerCharacterBase;

UCLASS()
class PROJECT4_API AP4Interactable : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AP4Interactable();

protected:

	/* This niagara component is created and assigned dynamically on StartNiagaraEffect via SpawnEmitterAttatched return value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visuals, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Visuals, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/* Actual Niagara System/Emitter */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Visuals)
		UNiagaraSystem* NiagaraEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behavior")
		bool bVFXOnConstruct;

public:	
	UFUNCTION(BlueprintCallable)
		virtual void StartNiagaraEffect();

	/* true = Successful interact, false = Unsuccessful (Inventory full, invalid state, etc.) */
	/* Used in Interact GA to start interacting with interactable classes */
	UFUNCTION(BlueprintCallable)
		virtual void OnInteract(const AP4PlayerCharacterBase* SourceActor, bool& Result);


	virtual void BeginDestroy() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
