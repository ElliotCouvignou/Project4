// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameplayEffect.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/P4ProjectileMovementComponent.h"
#include "P4AbilityProjectile.generated.h"

class AProject4Character;

UENUM(BlueprintType)
enum class EProjectileMovementType : uint8
{
	// 0 None
	SkillShot		UMETA(DisplayName = "SkillShot"),  // just simple 
	Homing			UMETA(DisplayName = "Homing"),    
	Cursor			UMETA(DisplayName = "Cursor")  // TODO: this type, make it follow cursor valorant style idk if this is necessary
};


//TODO: make another one of these for multiple people hit (return dynamic multicast of array of characters)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFirstCharacterHit, class AProject4Character*, Character);


UCLASS()
class PROJECT4_API AP4AbilityProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP4AbilityProjectile();

	virtual void PreInitializeComponents();

	/************************/
	/*   Expose on Spawn    */
	/************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
		EProjectileMovementType MovementType = EProjectileMovementType::SkillShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
		AProject4Character* HomingTarget;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* Hitbox;

	/* optional */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UP4ProjectileMovementComponent* ProjectileMovement;


	/* This niagara component is created and assigned dynamically on StartNiagaraEffect via SpawnEmitterAttatched return value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visuals, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NiagaraComponent;

	/* Actual Niagara System/Emitter to play on BeginPlay (leave empty if not needed) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Visuals)
		UNiagaraSystem* StartupNiagaraEffect;


	// Range or distance traveled of projectile ability
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		float Range;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		FGameplayEffectSpecHandle EffectSpecHandle;

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle DestroyActorTimerHandle;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnFirstCharacterHit OnFirstCharacterHitDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* IMplementation of onhit event */
	UFUNCTION(BlueprintNativeEvent)
		void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		virtual void OnHitboxBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
		FVector StartWorldPos;

public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
