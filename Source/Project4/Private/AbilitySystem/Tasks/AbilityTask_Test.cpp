// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Tasks/AbilityTask_Test.h"

UAbilityTask_Test* UAbilityTask_Test::CreateMyTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float examplevariable)
{
	UAbilityTask_Test* MyObj = NewAbilityTask<UAbilityTask_Test>(OwningAbility, TaskInstanceName);
	//Just assume we have defined a float called OptionalValue somewhere in the class before. This is just an example.
	MyObj->OptionalValue = examplevariable;

	return MyObj;
}

void UAbilityTask_Test::Activate()
{
	/* This is the part where you'd set up different delegates, timers etc. to prepare the task
	to eventually broadcast OnCalled sometime later. We have nothing prepared in this tutorial
	task however, so we may as well just call OnCalled right within the Activate function instead. */
	OnCalled.Broadcast(500.f, 42);
}
