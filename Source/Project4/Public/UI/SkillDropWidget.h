// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/SkillDropActor.h"
#include "SkillDropWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API USkillDropWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetupGeneralSkillPoolChoices(const TArray<EAbilityCategory>& SkillPools);


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetupAbilityChoicesForModifiers(const TArray<TSubclassOf<class UP4GameplayAbility>>& AbilityChoices);


	
};
