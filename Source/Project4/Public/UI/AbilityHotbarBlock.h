// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Project4.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilityHotbarBlock.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UAbilityHotbarBlock : public UUserWidget
{
	GENERATED_BODY()


public:
	UAbilityHotbarBlock(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability, Meta = (ExposeOnSpawn = true))
		int BlockIdx;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		FGameplayTagContainer CooldownTagContainer;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		class UAbilitySystemComponent* ASC;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		TSubclassOf<class UP4GameplayAbility> Ability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		FGameplayAbilitySpecHandle AbilitySpec;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Binding", Meta = (ExposeOnSpawn = true))
		EP4AbilityInputID InputBind;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Binding")
		FString InputBindActionName;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BindAbilityToBlock(TSubclassOf<class UP4GameplayAbility> NewAbility);

	UFUNCTION(BlueprintCallable)
		void SetAbilitySpecFromAbility();

	/**/
	UFUNCTION(BlueprintCallable)
		void InitializeInputCallback();

	void OnInputPressed();
	void OnInputReleased();
};
