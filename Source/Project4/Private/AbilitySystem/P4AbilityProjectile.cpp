// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/P4AbilityProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

// Sets default values
AP4AbilityProjectile::AP4AbilityProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bAlwaysRelevant = true;
	bOnlyRelevantToOwner = false;

	Hitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	RootComponent = Hitbox;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetGenerateOverlapEvents(false);

	FScriptDelegate OverlapDelegate;
	OverlapDelegate.BindUFunction(this, "OnHitboxBeginOverlap");
	Hitbox->OnComponentBeginOverlap.Add(OverlapDelegate);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->SetIsReplicated(true);
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void AP4AbilityProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartWorldPos = GetActorLocation();

	if (StartupNiagaraEffect)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(StartupNiagaraEffect, RootComponent, FName(TEXT("None")), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), EAttachLocation::KeepRelativeOffset, true);
	}
}

void AP4AbilityProjectile::OnHitboxBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

// Called every frame
void AP4AbilityProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

