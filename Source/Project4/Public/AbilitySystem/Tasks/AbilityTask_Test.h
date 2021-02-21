// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Test.generated.h"

// just some scratch paper stuff as I figure out how GAS works

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMyTwoParamDelegate, float, FirstParamName, int32, SecondParamName);
/**
 * 
 */
UCLASS()
class PROJECT4_API UAbilityTask_Test : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
		FMyTwoParamDelegate OnCalled;

public:

	UPROPERTY(BlueprintReadWrite)
		float OptionalValue;


		/* This UFUNCTION macro describes, in that order:
	The function can be called in BP, the category in which
	the function will de displayed in the BP-function-
	dropdown is "Ability", subcategory "Tasks", the function
	name in BP is displayed as "ExecuteMyTask",
	the pin for the parameter "OwningAbility" is hidden in BP
	and the parameter "OwningAbility" will default to
	the object the calling graph belongs to, if applicable.
	Finally, BlueprintInternalUseOnly = "TRUE" prevents
	a regular function node for this UFUNCTION to be created,
	which makes sense because this function needs to use
	an async task node instead(which has some added behaviour
	on being called such as actually activating the
	task, extra exec pins, etc). */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "ExecuteTestTask", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_Test* CreateMyTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float examplevariable);


	// overrides

	/* This function will call after the BP node has successfully requested the
	ability task from the static function. You put your actual
	functionality here. More on that in a bit. */
	virtual void Activate() override; 

};
