// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/SkillTree/SkillTreeDataAsset.h"
#include "SkillTreeNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API USkillTreeNodeWidget : public UUserWidget
{
	GENERATED_BODY()

public:



protected:

	/* Index/Node in skill tree data asset this widget node represents */
	/* Default at -1, this MUST be changed as it is invalid index */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Tree Node | Data Asset", Meta = (ExposeOnSpawn = true))
		int DataAssetIndex;

	virtual void NativeConstruct() override;
	
};
