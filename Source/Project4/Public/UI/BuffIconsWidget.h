// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "BuffIconsWidget.generated.h"


USTRUCT(BlueprintType)
struct FBuffIconDataStruct
{
	GENERATED_USTRUCT_BODY()

	FBuffIconDataStruct()
	{

	}

	/* Custom constructor */
	FBuffIconDataStruct(const FGameplayTag& Tag, const FGameplayEffectSpec& SpecApplied)
	{
		BuffTag = Tag;

		const UP4GameplayAbility* Ability = Cast<UP4GameplayAbility>(SpecApplied.GetEffectContext().GetAbility());
		IconImage = Ability->AbilityIcon;
		Duration = SpecApplied.GetDuration();
		TooltipText = Ability->BuffToolTipText;

		ResetDurationOnNewStack = SpecApplied.Def->StackDurationRefreshPolicy == EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
		Stackable = (SpecApplied.Def->StackLimitCount == 0 ? false : true);
		StacksExpireTogether = SpecApplied.Def->StackExpirationPolicy == EGameplayEffectStackingExpirationPolicy::ClearEntireStack;
		RestartOnExpire = SpecApplied.Def->StackExpirationPolicy == EGameplayEffectStackingExpirationPolicy::RefreshDuration;
	}

	UPROPERTY(BlueprintReadWrite)
		FGameplayTag BuffTag;

	UPROPERTY(BlueprintReadOnly)
		UTexture2D* IconImage;

	/* -1 = infinite duration */
	UPROPERTY(BlueprintReadWrite)
		float Duration;
	UPROPERTY(BlueprintReadWrite)
		FText TooltipText;

	/* GE Stack Duration Refresh Policy */
	UPROPERTY(BlueprintReadWrite)
		bool ResetDurationOnNewStack;

	/* Stack limit count > 0 */
	UPROPERTY(BlueprintReadWrite)
		bool Stackable;

	/* true if Stack expiration policy = "Clear entire stack" */
	UPROPERTY(BlueprintReadWrite)
		bool StacksExpireTogether;

	/* true if Stack expiration policy = "Refresh Duration" */
	UPROPERTY(BlueprintReadWrite)
		bool RestartOnExpire;
};


/**
 * 
 */
UCLASS()
class PROJECT4_API UBuffIconsWidget : public UUserWidget
{
	GENERATED_BODY()


public: 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NewBuffIconWithTimer(const FGameplayTag& Bufftag, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle& ActiveHandle);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateBuffStackCount(const FGameplayTag& BuffTag, int32 Count);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void RemoveBuffIconFromUI(const FGameplayTag& BuffTag);

	// Cant figure out how to call custom struct constructors in BP
	// Use this as alternative
	UFUNCTION(BlueprintCallable)
		FBuffIconDataStruct CreateBuffIconDataStruct(const FGameplayTag& Bufftag, const FGameplayEffectSpec& SpecApplied);


};
