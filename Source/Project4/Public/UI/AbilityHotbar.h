// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityHotbar.generated.h"



/**
 * 
 */
UCLASS()
class PROJECT4_API UAbilityHotbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UAbilityHotbar(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// for BP's to read
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		int AbilityBlockAmount;

	// ATM this gets set in bliueprint construct, Cant find equivalent in c++
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		class AProject4Character* PlayerRef;

	UFUNCTION(BlueprintCallable, Category = Abilities)
		void GivePlayerAbility(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability);
};
