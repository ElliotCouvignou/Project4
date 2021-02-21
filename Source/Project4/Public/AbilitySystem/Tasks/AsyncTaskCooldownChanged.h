// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AsyncTaskCooldownChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChanged, FGameplayTag, CooldownTag, float, TimeRemaining, float, Duration);

/**
 * Blueprint node to automatically register a listener for changes (Begin and End) to an array of Cooldown tags.
 * Useful to use in UI.
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class UAsyncTaskCooldownChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnCooldownChanged OnCooldownBegin;

	UPROPERTY(BlueprintAssignable)
		FOnCooldownChanged OnCooldownEnd;

	UPROPERTY(BlueprintAssignable)
		FOnCooldownChanged OnCooldownDurationChanged;

	// Listens for changes (Begin and End) to cooldown GameplayEffects based on the cooldown tag.
	// UseServerCooldown determines if the Sever's cooldown is returned in addition to the local predicted cooldown.
	// If using ServerCooldown, TimeRemaining and Duration will return -1 to signal local predicted cooldown has begun.
	// Set ExactTagsOnly to only listen for tags with exact name as input tag containers
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncTaskCooldownChanged* ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer CooldownTags, bool UseServerCooldown, bool ExactTagsOnly);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
		void EndTask();

protected:

	UPROPERTY()
		UAsyncTaskCooldownChanged* AsyncTaskCDChanged;


	UPROPERTY()
		UAbilitySystemComponent* ASC;

	FGameplayTagContainer CooldownTags;

	bool UseServerCooldown;
	bool bExactTagsOnly;

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void CooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount);

	void OnActiveGameplayEffectTimeChanged(FActiveGameplayEffectHandle ActiveGEHandle, float NewStartTime, float NewDuration);

	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);
};
