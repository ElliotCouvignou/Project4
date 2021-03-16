// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/P4Interactable.h"
#include "SkillDropActor.generated.h"

UENUM(BlueprintType)
enum class EBasicSkillCategories : uint8
{
	Physical		UMETA(DisplayName = "Physical"),
	Magical			UMETA(DisplayName = "Magical"),
	Defensive		UMETA(DisplayName = "Defensive"),
	Utility			UMETA(DisplayName = "Utility")
};

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

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnPlayerSkillDropInteracted(const AP4PlayerCharacterBase* Actor);
	void Server_OnPlayerSkillDropInteracted_Implementation(const AP4PlayerCharacterBase* Actor);
	bool Server_OnPlayerSkillDropInteracted_Validate(const AP4PlayerCharacterBase* Actor) { return true; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
