// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "GameplayTagContainer.h"
#include "P4TalentGraphNode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECT4_API UP4TalentGraphNode : public UGenericGraphNode
{
	GENERATED_BODY()

public:
	UP4TalentGraphNode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		FText TalentName;

	/* Gameplay Tags of all abilities that this talent relates with */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		FGameplayTagContainer RelevantAbilityGameplayTags;

	/* Ability Class this node will grant and rank up when interacted with
		This has the implementation of the talent as an ability */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		TSubclassOf<class UP4GameplayAbility> TalentAbilityClass;

	/* this is an optinal implementation of the talent, can be empty if ability class used instead */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		TSubclassOf<class UGameplayEffect> TalentGameplayEffectClass;

	/* TODO. this should have richtext format for ui to show */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		FText TalentTooltip;

#if WITH_EDITOR 

	virtual FText GetNodeTitle() const override;

	virtual void SetNodeTitle(const FText& NewTitle) override;

	virtual FLinearColor GetBackgroundColor() const override;
#endif
};
