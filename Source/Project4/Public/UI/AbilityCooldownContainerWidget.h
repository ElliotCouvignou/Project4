// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilityCooldownContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UAbilityCooldownContainerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void OnAbilityCooldownCommited(UGameplayAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnNewAbilityCooldown(UClass* AbilityClass);
};
