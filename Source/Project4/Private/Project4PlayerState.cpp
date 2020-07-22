// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Project4PlayerState.h"

#include "Characters/Project4Character.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "Project4Controller.h"

#include "AbilitySystem/PlayerAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystemComponent.h"

#include "UI/GameplayHudWidget.h"
#include "UI/FloatingStatusBarWidget.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AProject4PlayerState::AProject4PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UP4AbilitySystemComponent>(TEXT("AbilitySystem"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    
    AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 10.f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	BuffDebuffTag = FGameplayTag::RequestGameplayTag(FName("Buffs"));
}

UAbilitySystemComponent* AProject4PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPlayerAttributeSet* AProject4PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool AProject4PlayerState::IsAlive() const
{
	return AttributeSet->GetHealth() > 0.f;
}

void AProject4PlayerState::BeginPlay()
{
	Super::BeginPlay();

	//BindAbilityDelegates();
}

//void AProject4PlayerState::ShowAbilityConfirmCancelText(bool ShowText)
//{
//}


void AProject4PlayerState::BindAbilityDelegates()
{
	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &AProject4PlayerState::OnActiveGameplayEffectApplied);
		//AbilitySystemComponent->RegisterGameplayTagEvent(BuffDebuffTag).AddUObject(this, &AProject4PlayerState::OnBuffTagChanged);

		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AProject4PlayerState::HealthChanged);
		HealthMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthMaxAttribute()).AddUObject(this, &AProject4PlayerState::HealthMaxChanged);
		HealthRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthRegenAttribute()).AddUObject(this, &AProject4PlayerState::HealthRegenChanged);

		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &AProject4PlayerState::ManaChanged);
		ManaMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaMaxAttribute()).AddUObject(this, &AProject4PlayerState::ManaMaxChanged);
		ManaRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaRegenAttribute()).AddUObject(this, &AProject4PlayerState::ManaRegenChanged);
		
		EnduranceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceChanged);
		EnduranceMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceMaxAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceMaxChanged);
		EnduranceRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceRegenAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceRegenChanged);

	}
}






/*********************/
/* Delegate Handlers */
/*********************/


void AProject4PlayerState::OnActiveGameplayEffectApplied(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	// get tags
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());

	FGameplayTagContainer TagContainer = FGameplayTagContainer();
	SpecApplied.GetAllGrantedTags(TagContainer);

	if (TagContainer.HasTag(BuffDebuffTag) && HasAuthority())
	{
		// Buff GE, find some stats for UI update
		// Need: IconImage (in ability class default), Duration. TooltipText(with iconimage), Bufftag, bool buffresettimer 
		
		// Find buff tag in this GE
		TArray<FGameplayTag> TagArray;
		TagContainer.GetGameplayTagArray(TagArray);

		TArray<FGameplayTag> BuffTags;
		for (const FGameplayTag& tag : TagArray)
		{
			if (tag.ToString().Contains(BuffDebuffTag.ToString()) || tag.RequestDirectParent().ToString().Contains(BuffDebuffTag.ToString()))
			{
				BuffTags.Add(tag);
			}
		}
		
		// find reset policy
		bool BuffResetTimer = (SpecApplied.Def->StackDurationRefreshPolicy == EGameplayEffectStackingDurationPolicy::NeverRefresh ? false : true);
		bool Stackable = (SpecApplied.Def->StackLimitCount == 0 ? false : true);

		const UP4GameplayAbility* Ability = Cast<UP4GameplayAbility>(SpecApplied.GetEffectContext().GetAbility());
		
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		for (const FGameplayTag BuffTag : BuffTags)
		{
			int OwnerStackCount = AbilitySystemComponent->GetCurrentStackCount(ActiveHandle);
			if (OwnerStackCount == 1)
			{
				//PC->SendBuffIconToUI(Ability->AbilityIcon, SpecApplied.GetDuration(), Ability->BuffToolTipText, BuffTag, BuffResetTimer, Stackable);
				PC->SendBuffIconToUI(SpecApplied, ActiveHandle);
			}
			else
			{
				// Possible case of new GE sharing buff tag with another GE
				PC->UpdateBuffIconStacksUI(BuffTag, OwnerStackCount);
			}

			if (Stackable)
			{
				// create binding on GE tag for stack count changes 
				AbilitySystemComponent->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &AProject4PlayerState::OnBuffGameplayEffectStackChanged);
			}
			else if (SpecApplied.Def->DurationPolicy == EGameplayEffectDurationType::Infinite)
			{	
				if (!DelegatedBuffTags.Contains(BuffTag.ToString())) {
					AbilitySystemComponent->RegisterGameplayTagEvent(BuffTag).AddUObject(this, &AProject4PlayerState::OnBuffTagRemoved);
					DelegatedBuffTags.Add(BuffTag.ToString());
				}
			}
		}
	}
}

// Server only function (Client tag counts are bugged... IDK man.......)
void AProject4PlayerState::OnBuffTagRemoved(const FGameplayTag Tag, int32 Count)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());

	// delete buff icon in local player UI
	if (Count == 0)
	{
		PC->RemoveBuffIconFromUI(Tag);
	}
}

void AProject4PlayerState::OnBuffGameplayEffectStackChanged(FActiveGameplayEffectHandle ActiveHandle, int32 NewStackCount, int32 OldStackCount)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());

	const FGameplayTagContainer* TagContainer = AbilitySystemComponent->GetGameplayEffectTargetTagsFromHandle(ActiveHandle);

	if (TagContainer->HasTag(BuffDebuffTag))
	{
		TArray<FGameplayTag> TagArray;
		TagContainer->GetGameplayTagArray(TagArray);

		TArray<FGameplayTag> BuffTags;
		for (const FGameplayTag& tag : TagArray)
		{
			if (tag.ToString().Contains(BuffDebuffTag.ToString()) || tag.RequestDirectParent().ToString().Contains(BuffDebuffTag.ToString()))
			{
				PC->UpdateBuffIconStacksUI(tag, NewStackCount);
			}
		}
	}
}


void AProject4PlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	
	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar
	if (PChar)
	{
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetHealthPercentage(Health / AttributeSet->GetHealthMax());
		}
	}

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateHealth(Health);
		}
	}

	// if dead and no deathtag on player (true = ondeath transient)
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (PChar)
		{
			PChar->Die();
		}
	}
}

void AProject4PlayerState::HealthMaxChanged(const FOnAttributeChangeData& Data)
{
	float HealthMax = Data.NewValue;

	// update floating bar
	AProject4Character* PChar = Cast<AProject4Character>(GetPawn());
	if (PChar)
	{
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetHealthPercentage(AttributeSet->GetHealth() / HealthMax);
		}
	}

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateHealthMax(HealthMax);
		}
	}

}

void AProject4PlayerState::HealthRegenChanged(const FOnAttributeChangeData& Data)
{
	float HealthRegen = Data.NewValue;

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateHealthRegen(HealthRegen);
		}
	}
}

void AProject4PlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	float Mana = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	//  update floating status bar
	if (PChar)
	{
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetManaPercentage(Mana / AttributeSet->GetManaMax());
		}
	}

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateMana(Mana);
		}
	}
}

void AProject4PlayerState::ManaMaxChanged(const FOnAttributeChangeData& Data)
{
	float ManaMax = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar 
	if (PChar)
	{
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetManaPercentage(AttributeSet->GetMana() / ManaMax);
		}
	}

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateManaMax(ManaMax);
		}
	}
}

void AProject4PlayerState::ManaRegenChanged(const FOnAttributeChangeData& Data)
{
	float ManaRegen = Data.NewValue;

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateManaRegen(ManaRegen);
		}
	}
}

void AProject4PlayerState::EnduranceChanged(const FOnAttributeChangeData& Data)
{
	float Endurance = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// TODO: update floating status bar

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateEndurance(Endurance);
		}
	}
}

void AProject4PlayerState::EnduranceMaxChanged(const FOnAttributeChangeData& Data)
{
	float EnduranceMax = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// TODO: update floating status bar

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateEnduranceMax(EnduranceMax);
		}
	} 
}

void AProject4PlayerState::EnduranceRegenChanged(const FOnAttributeChangeData& Data)
{
	float EnduranceRegen = Data.NewValue;

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateEnduranceRegen(EnduranceRegen);
		}
	}
}

