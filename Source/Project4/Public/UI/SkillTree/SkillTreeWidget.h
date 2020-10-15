// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

/* each enum represents UI color of branch from dark grey - light grey */
UENUM(BlueprintType)
enum class ESkillTreeBranchState : uint8
{
	Unreachable		UMETA(DisplayName = "Unreachable"),
	Reachable		UMETA(DisplayName = "Reachable"),
	Reached			UMETA(DisplayName = "Reached") // currently unused as it requires alot of work for something minor in aesthetics
};

/**
 * 
 */
UCLASS()
class PROJECT4_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	/* Fill this out if this node grants a passive via GameplayEffect */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Tree Node | Data Asset", Meta = (ExposeOnSpawn = true))
		class USkillTreeDataAsset* SkillTreeDataAsset;
};
