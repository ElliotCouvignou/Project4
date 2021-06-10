// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AsyncTaskCooldownChanged.h"



#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)



UAsyncTaskCooldownChanged* UAsyncTaskCooldownChanged::ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer InCooldownTags, bool InUseServerCooldown, bool ExactTagsOnly)
{
	UAsyncTaskCooldownChanged* ListenForCooldownChange = NewObject<UAsyncTaskCooldownChanged>();
	ListenForCooldownChange->ASC = AbilitySystemComponent;
	ListenForCooldownChange->CooldownTags = InCooldownTags;
	ListenForCooldownChange->UseServerCooldown = InUseServerCooldown;
	ListenForCooldownChange->bExactTagsOnly = ExactTagsOnly;

	if (!IsValid(AbilitySystemComponent) || InCooldownTags.Num() < 1)
	{
		ListenForCooldownChange->EndTask();
		return nullptr;
	}

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(ListenForCooldownChange, &UAsyncTaskCooldownChanged::OnActiveGameplayEffectAddedCallback);
	

	TArray<FGameplayTag> CooldownTagArray;
	InCooldownTags.GetGameplayTagArray(CooldownTagArray);

	for (FGameplayTag CooldownTag : CooldownTagArray)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::AnyCountChange).AddUObject(ListenForCooldownChange, &UAsyncTaskCooldownChanged::CooldownTagChanged);
	}

	return ListenForCooldownChange;
}

void UAsyncTaskCooldownChanged::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);

		TArray<FGameplayTag> CooldownTagArray;
		CooldownTags.GetGameplayTagArray(CooldownTagArray);

		for (FGameplayTag CooldownTag : CooldownTagArray)
		{
			ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkPendingKill();
}

void UAsyncTaskCooldownChanged::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	TArray<FGameplayTag> CooldownTagArray;
	CooldownTags.GetGameplayTagArray(CooldownTagArray);

	
	
	for (FGameplayTag CooldownTag : CooldownTagArray)
	{
		bool FoundTag = (!bExactTagsOnly) ? (AssetTags.HasTag(CooldownTag) || GrantedTags.HasTag(CooldownTag)) : (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag));
		if (FoundTag)
		{
			float TimeRemaining = 0.0f;
			float Duration = 0.0f;
			// Expecting cooldown tag to always be first tag
			FGameplayTagContainer CooldownTagContainer(GrantedTags.GetByIndex(0));
			GetCooldownRemainingForTag(CooldownTagContainer, TimeRemaining, Duration);

			ASC->OnGameplayEffectRemovedDelegate(ActiveHandle)->AddUObject(this, &UAsyncTaskCooldownChanged::OnActiveGameplayEffectRemoved);

			//print(CooldownTag.ToString());
			
			if (ASC->GetOwnerRole() == ROLE_Authority)
			{
				// Player is Server
				OnCooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
			}
			else if (!UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				// Client using predicted cooldown
				OnCooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
			}
			else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated() == nullptr)
			{
				// Client using Server's cooldown. This is Server's corrective cooldown GE.
				OnCooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
			}
			else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				// Client using Server's cooldown but this is predicted cooldown GE.
				// This can be useful to gray out abilities until Server's cooldown comes in.
				OnCooldownBegin.Broadcast(CooldownTag, -1.0f, -1.0f);
			}

			// From Active GE Handle, get active GE to bind to  FActiveGameplayEffectEvents::FOnActiveGameplayEffectTimeChange
			// This binding will be fired when the CD duration is manually changed but not 100% refunded (e.g CD refund from an effect).
			const FActiveGameplayEffect* ActiveGE = Target->GetActiveGameplayEffect(ActiveHandle);
			ASC->OnGameplayEffectTimeChangeDelegate(ActiveHandle)->AddUObject(this, &UAsyncTaskCooldownChanged::OnActiveGameplayEffectTimeChanged);
		
		}
	} 
}

void UAsyncTaskCooldownChanged::CooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		OnCooldownEnd.Broadcast(CooldownTag, -1.0f, -1.0f);
	}
}

void UAsyncTaskCooldownChanged::OnActiveGameplayEffectRemoved()
{
	OnCooldownEnd.Broadcast(FGameplayTag(), 0.f, -1.0f);
}

void UAsyncTaskCooldownChanged::OnActiveGameplayEffectTimeChanged(FActiveGameplayEffectHandle ActiveGEHandle, float NewStartTime, float NewDuration)
{
	// really only need to send NewDuration but since I'm hijacking an already existing delegate
	// I have to fill in other values with redundancy or junk
	OnCooldownDurationChanged.Broadcast(FGameplayTag(), NewDuration, NewDuration);
}

bool UAsyncTaskCooldownChanged::GetCooldownRemainingForTag(FGameplayTagContainer InCooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (IsValid(ASC) && InCooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
}