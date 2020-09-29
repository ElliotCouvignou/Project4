// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameplayEffect.h"
#include "Components/CapsuleComponent.h"
#include "P4AbilityProjectile.generated.h"

UCLASS()
class PROJECT4_API AP4AbilityProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP4AbilityProjectile();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* Hitbox;

	/* optional */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovement;

	/* This niagara component is created and assigned dynamically on StartNiagaraEffect via SpawnEmitterAttatched return value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visuals, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NiagaraComponent;

	/* Actual Niagara System/Emitter to play on BeginPlay (leave empty if not needed) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Visuals)
		UNiagaraSystem* StartupNiagaraEffect;


	// Range or distance traveled of projectile ability
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Range;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		FGameplayEffectSpecHandle EffectSpecHandle;

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle DestroyActorTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
		void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		virtual void OnHitboxBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
		FVector StartWorldPos;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
