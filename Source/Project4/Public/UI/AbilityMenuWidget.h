// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericGraph/Abilities/P4AbilityPoolGraph.h"
#include "AbilityMenuWidget.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT4_API UAbilityMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public: 
	UAbilityMenuWidget(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities | Pools")
		UP4AbilityPoolGraph* GlobalAbilityPoolGraph;
};
